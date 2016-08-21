#include "stdafx.h"

#include "DeferredRenderer.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "Synchronizer.h"

#include "DirectionalLight.h"
#include "PointLight.h"

#include "Sprite.h"

#include "EmitterInstance.h"

#include "SkySphere.h"
#include "Terrain.h"

#include "Line3D.h"
#include "LightPass.h"

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
		myText = new CText("Arial.ttf", 14, 1);

		myTimeManager->GetTimer(loadTimer).Update();
		loadTime = myTimeManager->GetTimer(loadTimer).GetTotalTime().GetMilliseconds() - loadTime;
		FONT_LOG("Font Took : %fms to load.", loadTime);

		myPointLight = new CPointLight();
		myDeferredRenderer = new CDeferredRenderer();
		myDepthTexture = new CTexture();
		myDepthTexture->InitStencil(CEngine::GetInstance()->GetWindowSize().myWidth, CEngine::GetInstance()->GetWindowSize().myHeight);
		myDepthTexture->SetDebugName("myDepthTexture");

		mySkysphere = new CSkySphere("Data/Model/Skysphere/SM_Skysphere.fbx", "Data/Shaders/T_Skysphere.json", aCamera);

		mySprite = new CSprite();
		mySprite->Initiate("Data/Textures/colors.dds", CU::Vector2f(256.f, 256.f), CU::Vector2f(0.f, 0.f));

		myEngine = CEngine::GetInstance();
		myDirectX = myEngine->GetDirectX();

		my3DLine = new CLine3D();
		my3DLine->Initiate();

		myLightPass = new CLightPass(myDeferredRenderer->GetGBuffer());




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
		//SAFE_DELETE(myDirectionalLight);
	}

	void CRenderer::Add2DCamera(CCamera* aCamera)
	{
		my2DCamera = aCamera;
	}

	void CRenderer::Render()
	{
		myEngine->Clear();

		Render3DCommands();
		myDepthTexture->CopyData(myDeferredRenderer->GetDepthStencil()->GetDepthTexture());
		myDeferredRenderer->UpdateConstantBuffer(myPrevFrame, myCamera->GetProjection());
		myDeferredRenderer->DeferredRender(); /* Ambient pass */

		//RenderLightCommands();
		RenderPointlight();
		//RenderSpotlight();

		myEngine->ResetRenderTargetAndDepth();
		mySkysphere->Render(myPrevFrame, myDepthTexture);
		myDeferredRenderer->Finalize();

		//RenderParticles();
		RenderLines();

		Render2DCommands();

		myEngine->Present();

		mySynchronizer.WaitForLogic();
		mySynchronizer.SwapBuffer();
		mySynchronizer.RenderIsDone();
		myPrevFrame = myCamera->GetOrientation();
	}

	void CRenderer::AddTerrain(CTerrain* someTerrain)
	{
		myTerrainArray.Add(someTerrain);
	}

	void CRenderer::Render3DCommands()
	{
		myDirectX->SetDepthBufferState(eDepthStencil::MASK_TEST);
		myDeferredRenderer->SetTargets();

		const CU::GrowingArray<SRenderCommand>& commands = mySynchronizer.GetRenderCommands(eCommandBuffer::e3D);
		for each(const SRenderCommand& command in commands)
		{
			switch (command.myType)
			{
				case eType::MODEL:
				{
					myDirectX->SetRasterizer(eRasterizer::CULL_BACK);
					myDirectX->SetBlendState(eBlendStates::BLEND_FALSE);

					CModel* model = myEngine->GetModel(command.myModelKey);

					model->SetPosition(command.myPosition);
					model->Render(myPrevFrame, myCamera->GetProjection());
				}break;
				case eType::SKYSPHERE:
				{
					mySkysphere->SetPosition(command.myPosition);
				}break;
				case eType::TERRAIN:
				{
					for (CTerrain* terrain : myTerrainArray)
					{
						terrain->Render(myPrevFrame, myCamera->GetProjection());
					}
				}break;
			}
		}
	}

	void CRenderer::Render2DCommands()
	{
		const CU::GrowingArray<SRenderCommand>& commands2D = mySynchronizer.GetRenderCommands(eCommandBuffer::e2D);
		myDirectX->SetRasterizer(eRasterizer::CULL_NONE);
		myDirectX->SetDepthBufferState(eDepthStencil::Z_DISABLED);
		for each(const SRenderCommand& command in commands2D)
		{
			switch (command.myType)
			{
				case eType::TEXT:
				{
					myText->SetText(command.myTextToPrint);
					myText->SetPosition({ command.myPosition.x, command.myPosition.y });
					myText->Render(my2DCamera);
				}break;
				case eType::SPRITE:
				{
					mySprite->Render(my2DCamera);
				}break;
			};
		}
		myDirectX->SetDepthBufferState(eDepthStencil::Z_ENABLED);
		myDirectX->SetRasterizer(eRasterizer::CULL_BACK);
	}


	void CRenderer::RenderSpotlight()
	{
		const CU::GrowingArray<SRenderCommand>& commands = mySynchronizer.GetRenderCommands(eCommandBuffer::eSpotlight);
		myDirectX->SetRasterizer(eRasterizer::CULL_NONE);
		myDirectX->SetDepthBufferState(eDepthStencil::READ_NO_WRITE);


		//myDeferredRenderer->SetLightShaders();

		for each(const SRenderCommand& command in commands)
		{
			DL_ASSERT_EXP(command.myType == eType::SPOTLIGHT, "Wrong command type in spotlight buffer.");
			myDirectX->SetBlendState(eBlendStates::LIGHT_BLEND);
			myPointLight->SetPosition(command.myPosition);
			myPointLight->SetRange(command.myRange);
			myPointLight->SetColor(CU::Vector4f(command.myColor.r, command.myColor.g, command.myColor.b, command.myIntensity));
			myPointLight->Update();
			//myDeferredRenderer->RenderLight(myPointLight, myCamera, myPrevFrame);
		}
		//myDeferredRenderer->DeactivateLight();
		myDirectX->SetDepthBufferState(eDepthStencil::Z_ENABLED);
		myDirectX->SetRasterizer(eRasterizer::CULL_BACK);
	}

	void CRenderer::RenderPointlight()
	{
		const CU::GrowingArray<SRenderCommand>& commands = mySynchronizer.GetRenderCommands(eCommandBuffer::ePointlight);

		myDirectX->SetRasterizer(eRasterizer::CULL_NONE);
		myDirectX->SetDepthBufferState(eDepthStencil::READ_NO_WRITE);
		CEffect* effect = myLightPass->GetPointlightEffect();
		effect->Activate();

		for each(const SRenderCommand& command in commands)
		{
			DL_ASSERT_EXP(command.myType == eType::POINTLIGHT, "Wrong command type in pointlight buffer.");
			myDirectX->SetBlendState(eBlendStates::LIGHT_BLEND);
			myPointLight->SetPosition(command.myPosition);
			myPointLight->SetRange(command.myRange);
			myPointLight->SetColor(CU::Vector4f(command.myColor.r, command.myColor.g, command.myColor.b, command.myIntensity));
			myPointLight->Update();
			myLightPass->RenderPointlight(myPointLight, myCamera, myPrevFrame);
		}
		effect->Deactivate();

		myDirectX->SetDepthBufferState(eDepthStencil::Z_ENABLED);
		myDirectX->SetRasterizer(eRasterizer::CULL_BACK);
	}

	void CRenderer::RenderParticles()
	{
		myDirectX->SetBlendState(eBlendStates::ALPHA_BLEND);
		myDirectX->SetRasterizer(eRasterizer::CULL_NONE);
		const CU::GrowingArray<SRenderCommand>& commands = mySynchronizer.GetRenderCommands(eCommandBuffer::eParticle);
		for each(const SRenderCommand& command in commands)
		{
			switch (command.myType)
			{
				case eType::PARTICLE:
				{
					//command.myEmitterInstance->Render(myCamera, myDepthTexture);
				}break;
			}
		}
		myDirectX->SetRasterizer(eRasterizer::CULL_BACK);
	}

	void CRenderer::RenderLines()
	{
		myDirectX->SetBlendState(eBlendStates::NO_BLEND);
		myDirectX->SetRasterizer(eRasterizer::CULL_NONE);
		myDirectX->SetDepthBufferState(eDepthStencil::Z_DISABLED);

		const CU::GrowingArray<SRenderCommand>& commands = mySynchronizer.GetRenderCommands(eCommandBuffer::eLine);
		for each(const SRenderCommand& command in commands)
		{
			switch (command.myType)
			{
				case eType::LINE:
				{
					my3DLine->Update(command.firstPoint, command.secondPoint);
					my3DLine->Render(myPrevFrame, myCamera->GetProjection());
				}break;
			}
		}
		myDirectX->SetBlendState(eBlendStates::NO_BLEND);
		myDirectX->SetDepthBufferState(eDepthStencil::Z_ENABLED);
		myDirectX->SetRasterizer(eRasterizer::CULL_BACK);
	}

};