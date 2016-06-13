#include "stdafx.h"
#include <thread>
#include "Renderer.h"
#include "Engine.h"
#include "Synchronizer.h"
#include "RenderCommand.h"
#include "Instance.h"
#include "Camera.h"
#include "DeferredRenderer.h"
#include "PointLight.h"
#include "EmitterInstance.h"
#include <TimeManager.h>
#include "SkySphere.h"
#include "Model.h"

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
		myText = new CText("Arial.ttf", 8, 1);

		myTimeManager->GetTimer(loadTimer).Update();
		loadTime = myTimeManager->GetTimer(loadTimer).GetTotalTime().GetMilliseconds() - loadTime;
		FONT_LOG("Font Took : %fms to load.", loadTime);

		myPointLight = new CPointLight();
		myDeferredRenderer = new CDeferredRenderer();
		myDepthTexture = new CTexture();
		myDepthTexture->InitAsDepthBuffer(CEngine::GetInstance()->GetWindowSize().myWidth, CEngine::GetInstance()->GetWindowSize().myHeight);
		mySkysphere = new CSkySphere("Data/Model/Skysphere/SM_Skysphere.fbx", "Data/Shaders/Skysphere.fx", aCamera);
	}

	CRenderer::~CRenderer()
	{
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
		std::stringstream textTime;
		textTime << "Render : " << myText->GetRenderTime() << "\nUpdate : " << myText->GetUpdateTime();
		mySynchronizer.AddRenderCommand(SRenderCommand(textTime.str(), CU::Vector2f(1920 - 200, 500)));
		CEngine::Clear();

		myDeferredRenderer->SetTargets();
		Render3DCommands();
		myDepthTexture->CopyData(myDeferredRenderer->GetDepthStencil()->GetDepthTexture());
		myDeferredRenderer->SetBuffers();
		myDeferredRenderer->DeferredRender();

		myDeferredRenderer->SetLightState(myCamera);
		RenderLightCommands();
		myDeferredRenderer->SetNormalState();

		RenderParticles();

		Render2DCommands();


		CEngine::Present();
		mySynchronizer.WaitForLogic();
		mySynchronizer.SwapBuffer();
		mySynchronizer.RenderIsDone();
		myPrevFrame = myCamera->GetOrientation();
	}


	void CRenderer::AddModel(CModel* aModel, const std::string& aModelKey)
	{
		myModels[aModelKey] = aModel;
	}

	void CRenderer::Render3DCommands()
	{
		const CU::GrowingArray<SRenderCommand>& commands = mySynchronizer.GetRenderCommands(eCommandType::e3D);
		for each(const SRenderCommand& command in commands)
		{
			switch (command.myType)
			{
			case SRenderCommand::eType::MODEL:
				myModels[command.myModelKey]->SetPosition(command.myPosition);
				myModels[command.myModelKey]->GetEffect()->SetMatrices(myModels[command.myModelKey]->GetOrientation(), myPrevFrame, myCamera->GetProjection());
				myModels[command.myModelKey]->Render();
				break;
			case SRenderCommand::eType::SKYSPHERE:
				mySkysphere->SetPosition(command.myPosition);
				mySkysphere->Render(myPrevFrame);
				break;
			}
		}
	}
	 
	void CRenderer::Render2DCommands()
	{
		const CU::GrowingArray<SRenderCommand>& commands2D = mySynchronizer.GetRenderCommands(eCommandType::e2D);
		CEngine::GetDirectX()->DisableZBuffer();
		for each(const SRenderCommand& command in commands2D)
		{
			switch (command.myType)
			{
			case SRenderCommand::eType::TEXT:
				myText->SetText(command.myTextToPrint);
				myText->SetPosition({ command.myPosition.x, command.myPosition.y });
				myText->Render(my2DCamera);
				break;
			}
		}
		CEngine::GetDirectX()->SetRasterizer(eRasterizer::CULL_BACK);
		CEngine::GetDirectX()->EnableZBuffer();

	}

	void CRenderer::RenderLightCommands()
	{
		const CU::GrowingArray<SRenderCommand>& commands = mySynchronizer.GetRenderCommands(eCommandType::LIGHT);
		for each(const SRenderCommand& command in commands)
		{
			switch (command.myType)
			{
			case SRenderCommand::eType::POINTLIGHT:
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
		CEngine::GetDirectX()->SetBlendState(eBlendStates::ALPHA_BLEND);
		CEngine::GetDirectX()->SetRasterizer(eRasterizer::CULL_NONE);
		const CU::GrowingArray<SRenderCommand>& commands = mySynchronizer.GetRenderCommands(eCommandType::PARTICLE);
		for each(const SRenderCommand& command in commands)
		{
			switch (command.myType)
			{
			case SRenderCommand::eType::PARTICLE:
				command.myEmitterInstance->Render(myCamera, myDepthTexture);
				break;
			}
		}
		CEngine::GetDirectX()->SetRasterizer(eRasterizer::CULL_BACK);
	}
};