#include "stdafx.h"
#include "Renderer.h"
#include "Synchronizer.h"
#include "RenderCommand.h"
#include "DeferredRenderer.h"
#include "EmitterInstance.h"
#include "SkySphere.h"
#include "PointLight.h"
#include "Sprite.h"
#include "Line3D.h"
namespace Snowblind
{

	CRenderer::CRenderer(CSynchronizer& aSynchronizer, CCamera* aCamera)
		: mySynchronizer(aSynchronizer)
		, myCamera(aCamera)
	{
		myTimeManager = new CU::TimeManager();
		int loadTimer = myTimeManager->CreateTimer();
		myTimeManager->GetTimer(loadTimer).Update();
		float loadTime = myTimeManager->GetTimer(loadTimer).GetTotalTime().GetMilliseconds();
		myText = new CText("Arial.ttf", 14, 0);

		myTimeManager->GetTimer(loadTimer).Update();
		loadTime = myTimeManager->GetTimer(loadTimer).GetTotalTime().GetMilliseconds() - loadTime;
		FONT_LOG("Font Took : %fms to load.", loadTime);

		myPointLight = new CPointLight();
		myDeferredRenderer = new CDeferredRenderer();
		myDepthTexture = new CTexture();
		myDepthTexture->InitAsDepthBuffer(CEngine::GetInstance()->GetWindowSize().myWidth, CEngine::GetInstance()->GetWindowSize().myHeight);

		mySkysphere = new CSkySphere("Data/Model/Skysphere/SM_Skysphere.fbx", "Data/Shaders/T_Skysphere.json", aCamera);

		mySprite = new CSprite();
		mySprite->Initiate("Data/Textures/colors.dds", CU::Vector2f(256.f, 256.f), CU::Vector2f(0.f, 0.f));

		myEngine = CEngine::GetInstance();
		myDirectX = myEngine->GetDirectX();


		my3DLine = new CLine3D();
		my3DLine->Initiate();


		//my3DLine->AddCube({ 0.f,0.f,0.f }, { 1.f,1.f,1.f });

	}

	CRenderer::~CRenderer()
	{
		SAFE_DELETE(my3DLine);
		SAFE_DELETE(mySprite);
		SAFE_DELETE(mySkysphere);
		SAFE_DELETE(myTimeManager);
		SAFE_DELETE(myDepthTexture);
		SAFE_DELETE(my2DCamera);
		SAFE_DELETE(myDeferredRenderer);
		SAFE_DELETE(myText);
		SAFE_DELETE(myPointLight);
	}

	void CRenderer::Add2DCamera(CCamera* aCamera)
	{
		my2DCamera = aCamera;
	}

	void CRenderer::Render()
	{
		myEngine->Clear();

		myDeferredRenderer->SetTargets();
		Render3DCommands();
		myDepthTexture->CopyData(myDeferredRenderer->GetDepthStencil()->GetDepthTexture());
		myDeferredRenderer->SetBuffers();
		myDeferredRenderer->DeferredRender();

		myDeferredRenderer->SetLightStates();
		RenderLightCommands();
		myDeferredRenderer->SetNormalStates();

		myDeferredRenderer->ResetBackbufferAndDepth();
		mySkysphere->SetPosition(mySpherePos);

		myDirectX->SetDepthBufferState(eDepthStencil::Z_DISABLED);
		myDirectX->SetRasterizer(eRasterizer::CULL_NONE);

		mySkysphere->Render(myPrevFrame);
		myDeferredRenderer->Finalize();

		myDirectX->SetRasterizer(eRasterizer::CULL_BACK);
		myDirectX->SetDepthBufferState(eDepthStencil::Z_ENABLED);
		//RenderParticles();

		RenderLines();

		Render2DCommands();

		myEngine->Present();

		mySynchronizer.WaitForLogic();
		mySynchronizer.SwapBuffer();
		mySynchronizer.RenderIsDone();
		myPrevFrame = myCamera->GetOrientation();
	}

	void CRenderer::Render3DCommands()
	{
		const CU::GrowingArray<SRenderCommand>& commands = mySynchronizer.GetRenderCommands(eCommandType::e3D);
		for each(const SRenderCommand& command in commands)
		{
			switch (command.myType)
			{
			case SRenderCommand::eType::MODEL:
			{
				myDirectX->SetRasterizer(eRasterizer::CULL_BACK);
				myDirectX->SetBlendState(eBlendStates::NO_BLEND);

				CModel* model = myEngine->GetModel(command.myModelKey);

				model->SetPosition(command.myPosition);
				model->Render(myPrevFrame, myCamera->GetProjection());

				break;
			}
			case SRenderCommand::eType::SKYSPHERE:
			{
				mySpherePos = command.myPosition;
				break;
			}
			}
		}
	}

	void CRenderer::Render2DCommands()
	{
		const CU::GrowingArray<SRenderCommand>& commands2D = mySynchronizer.GetRenderCommands(eCommandType::e2D);
		myDirectX->SetRasterizer(eRasterizer::CULL_NONE);
		myDirectX->SetDepthBufferState(eDepthStencil::Z_DISABLED);
		for each(const SRenderCommand& command in commands2D)
		{
			switch (command.myType)
			{
			case SRenderCommand::eType::TEXT:

				myText->SetText(command.myTextToPrint);
				myText->SetPosition({ command.myPosition.x, command.myPosition.y });
				myText->Render(my2DCamera);
				break;
			case SRenderCommand::eType::SPRITE:
				mySprite->Render(my2DCamera);
				break;
			}
		}
		myDirectX->SetDepthBufferState(eDepthStencil::Z_ENABLED);
		myDirectX->SetRasterizer(eRasterizer::CULL_BACK);

	}

	void CRenderer::RenderLightCommands()
	{
		const CU::GrowingArray<SRenderCommand>& commands = mySynchronizer.GetRenderCommands(eCommandType::LIGHT);
		for each(const SRenderCommand& command in commands)
		{
			switch (command.myType)
			{
			case SRenderCommand::eType::POINTLIGHT:

				myDirectX->SetBlendState(eBlendStates::ALPHA_BLEND);
				myPointLight->SetPosition(command.myPosition);
				myPointLight->SetRange(command.myRange);
				myPointLight->SetColor(CU::Vector4f(command.myColor.r, command.myColor.g, command.myColor.b, command.myIntensity));
				myPointLight->Update();
				myDeferredRenderer->RenderLight(myPointLight, myCamera, myPrevFrame);
				break;
			}
		}
	}

	void CRenderer::RenderParticles()
	{
		myDirectX->SetBlendState(eBlendStates::ALPHA_BLEND);
		myDirectX->SetRasterizer(eRasterizer::CULL_NONE);
		const CU::GrowingArray<SRenderCommand>& commands = mySynchronizer.GetRenderCommands(eCommandType::PARTICLE);
		for each(const SRenderCommand& command in commands)
		{
			switch (command.myType)
			{
			case SRenderCommand::eType::PARTICLE:
				//command.myEmitterInstance->Render(myCamera, myDepthTexture);
				break;
			}
		}
		myDirectX->SetRasterizer(eRasterizer::CULL_BACK);
	}

	void CRenderer::RenderLines()
	{
		const CU::GrowingArray<SRenderCommand>& commands = mySynchronizer.GetRenderCommands(eCommandType::LINE);
		for each(const SRenderCommand& command in commands)
		{
			switch (command.myType)
			{
			case SRenderCommand::eType::LINE:
			{
				my3DLine->Update(command.firstPoint, command.secondPoint);
				my3DLine->Render(myPrevFrame, myCamera->GetProjection());
				break;
			}
			}
		}
	}

};