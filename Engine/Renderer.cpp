#include "stdafx.h"

#include "DeferredRenderer.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "Synchronizer.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "Sprite.h"

#include "EmitterInstance.h"

#include "SkySphere.h"
#include "Terrain.h"

#include "Line3D.h"
#include "LightPass.h"
namespace Snowblind
{
	bool CRenderer::Initiate(CSynchronizer* synchronizer, CCamera* camera_3d, CCamera* camera_2d)
	{
		mySynchronizer = synchronizer;
		if (!mySynchronizer)
			return false;
		myCamera = camera_3d;
		if (!myCamera)
			return false;
		my2DCamera = camera_2d;
		if (!my2DCamera)
			return false;
		myText = new CText("Data/Font/OpenSans-Bold.ttf", 8, 1);
		if (!myText)
			return false;
		//FONT_LOG("Font Took : %fms to load.", loadTime);

		myPointLight = new CPointLight; //Where should this live?
		if (!myPointLight)
			return false;
		mySpotlight = new CSpotLight; // Where should this live?
		if (!mySpotlight)
			return false;
		myDeferredRenderer = new CDeferredRenderer; // Where should this live?
		if (!myDeferredRenderer)
			return false;
		myDepthTexture = new CTexture; //Where should this live?
		if (!myDepthTexture)
			return false;

		myDepthTexture->InitStencil(CEngine::GetInstance()->GetWindowSize().myWidth, CEngine::GetInstance()->GetWindowSize().myHeight);
		myDepthTexture->SetDebugName("myDepthTexture");

		mySkysphere = new CSkySphere("Data/Model/Skysphere/SM_Skysphere.fbx", "Data/Shaders/T_Skysphere.json", camera_3d);
		if (!mySkysphere)
			return false;
		//mySprite = new CSprite;
		//mySprite->Initiate("Data/Textures/colors.dds", CU::Vector2f(256.f, 256.f), CU::Vector2f(0.f, 0.f));

		myEngine = CEngine::GetInstance();
		if (!myEngine)
			return false;
#ifdef SNOWBLIND_DX11
		myDirectX = myEngine->GetAPI();
#endif
		my3DLine = new CLine3D; //Where should this live?
		if (!my3DLine)
			return false;

		my3DLine->Initiate();

		myLightPass = new CLightPass(myDeferredRenderer->GetGBuffer());
		if (!myLightPass)
			return false;

		return true;
	}

	bool CRenderer::CleanUp()
	{
		SAFE_DELETE(my3DLine);
		SAFE_DELETE(mySprite);
		SAFE_DELETE(mySkysphere);
		SAFE_DELETE(myDepthTexture);
		SAFE_DELETE(my2DCamera);
		SAFE_DELETE(myDeferredRenderer);
		SAFE_DELETE(myText);

		SAFE_DELETE(myPointLight);
		SAFE_DELETE(mySpotlight);
		SAFE_DELETE(myLightPass);
		return true;
	}

	void CRenderer::Add2DCamera(CCamera* aCamera)
	{
		my2DCamera = aCamera;
	}

	void CRenderer::Render()
	{

		myEngine->Clear();
#ifdef SNOWBLIND_DX11
		Render3DCommands();

		myDepthTexture->CopyData(myDeferredRenderer->GetDepthStencil()->GetDepthTexture());
		myDeferredRenderer->DeferredRender(myPrevFrame, myCamera->GetProjection());

		RenderPointlight();
		RenderSpotlight();

		myEngine->ResetRenderTargetAndDepth();
		mySkysphere->Render(myPrevFrame, myDepthTexture);
		myDeferredRenderer->Finalize();

		//RenderParticles();
		RenderLines();

		Render2DCommands();
#endif
		myEngine->Present();

		mySynchronizer->WaitForLogic();
		mySynchronizer->SwapBuffer();
		mySynchronizer->RenderIsDone();
		myPrevFrame = myCamera->GetOrientation();
	}

	void CRenderer::AddTerrain(CTerrain* someTerrain)
	{
		myTerrainArray.Add(someTerrain);
	}

	void CRenderer::Render3DCommands()
	{
#ifdef SNOWBLIND_DX11
		myDirectX->SetDepthBufferState(eDepthStencil::MASK_TEST);
		myDeferredRenderer->SetTargets();

		const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::e3D);
		for each(const RenderCommand& command in commands)
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
					mySkysphere->SetPosition(command.myPosition);//only updating the position here because it's supposed to be rendered after the light and be unaffected by the ambient pass
				}break;
				case eType::TERRAIN:
				{
					myDirectX->SetRasterizer(eRasterizer::CULL_BACK);
					myDirectX->SetBlendState(eBlendStates::BLEND_FALSE);
					for (CTerrain* terrain : myTerrainArray)
					{
						terrain->Render(myPrevFrame, myCamera->GetProjection());
					}
				}break;
			}
		}
#endif
	}

	void CRenderer::Render2DCommands()
	{
#ifdef SNOWBLIND_DX11
		const CU::GrowingArray<RenderCommand>& commands2D = mySynchronizer->GetRenderCommands(eCommandBuffer::e2D);
		myDirectX->SetRasterizer(eRasterizer::CULL_NONE);
		myDirectX->SetDepthBufferState(eDepthStencil::Z_DISABLED);
		for each(const RenderCommand& command in commands2D)
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
#endif
	}

	void CRenderer::RenderSpotlight()
	{
#ifdef SNOWBLIND_DX11
		const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::eSpotlight);
		myDirectX->SetRasterizer(eRasterizer::CULL_NONE);
		myDirectX->SetDepthBufferState(eDepthStencil::READ_NO_WRITE);
		CEffect* effect = myLightPass->GetSpotlightEffect()	;
		effect->Activate();

		for each(const RenderCommand& command in commands)
		{
			DL_ASSERT_EXP(command.myType == eType::SPOTLIGHT, "Wrong command type in spotlight buffer.");
			myDirectX->SetBlendState(eBlendStates::LIGHT_BLEND);

			mySpotlight->SetPosition(command.myPosition);
			mySpotlight->SetRange(command.myRange);
			mySpotlight->SetColor(CU::Vector4f(command.myColor.r, command.myColor.g, command.myColor.b, 1));
			mySpotlight->SetAngle(command.myAngle);

			mySpotlight->DoTranslation(command.myRotationMatrix);

			myLightPass->RenderSpotlight(mySpotlight, myCamera, myPrevFrame);
		}
		effect->Deactivate();
		myDirectX->SetDepthBufferState(eDepthStencil::Z_ENABLED);
		myDirectX->SetRasterizer(eRasterizer::CULL_BACK);
#endif
	}

	void CRenderer::RenderPointlight()
	{
#ifdef SNOWBLIND_DX11
		const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::ePointlight);

		myDirectX->SetRasterizer(eRasterizer::CULL_NONE);
		myDirectX->SetDepthBufferState(eDepthStencil::READ_NO_WRITE);
		CEffect* effect = myLightPass->GetPointlightEffect();
		effect->Activate();

		for each(const RenderCommand& command in commands)
		{
			DL_ASSERT_EXP(command.myType == eType::POINTLIGHT, "Wrong command type in pointlight buffer.");
			myDirectX->SetBlendState(eBlendStates::LIGHT_BLEND);
			myPointLight->SetPosition(command.myPosition);
			myPointLight->SetRange(command.myRange);
			myPointLight->SetColor(CU::Vector4f(command.myColor.r, command.myColor.g, command.myColor.b, 1));
			myPointLight->Update();
			myLightPass->RenderPointlight(myPointLight, myCamera, myPrevFrame);
		}
		effect->Deactivate();

		myDirectX->SetDepthBufferState(eDepthStencil::Z_ENABLED);
		myDirectX->SetRasterizer(eRasterizer::CULL_BACK);
#endif
	}

	void CRenderer::RenderParticles()
	{
#ifdef SNOWBLIND_DX11
		myDirectX->SetBlendState(eBlendStates::ALPHA_BLEND);
		myDirectX->SetRasterizer(eRasterizer::CULL_NONE);
		const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::eParticle);
		for each(const RenderCommand& command in commands)
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
#endif
	}

	void CRenderer::RenderLines()
	{
#ifdef SNOWBLIND_DX11
		myDirectX->SetBlendState(eBlendStates::NO_BLEND);
		myDirectX->SetRasterizer(eRasterizer::CULL_NONE);

		ID3D11RenderTargetView* backbuffer = myDirectX->GetBackbuffer();
		ID3D11DepthStencilView* depth = myDeferredRenderer->GetDepthStencil()->GetDepthView();
		myDirectX->GetContext()->OMSetRenderTargets(1, &backbuffer, depth);

		const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::eLine);
		for each(const RenderCommand& command in commands)
		{
			switch (command.myType)
			{
				case eType::LINE_Z_ENABLE:
				{
					myDirectX->SetDepthBufferState(eDepthStencil::Z_ENABLED);
					my3DLine->Update(command.firstPoint, command.secondPoint);
					my3DLine->Render(myPrevFrame, myCamera->GetProjection());
				}break;
				case eType::LINE_Z_DISABLE:
				{
					myDirectX->SetDepthBufferState(eDepthStencil::Z_DISABLED);
					my3DLine->Update(command.firstPoint, command.secondPoint);
					my3DLine->Render(myPrevFrame, myCamera->GetProjection());
				}break;
			}
		}
		myDirectX->SetBlendState(eBlendStates::NO_BLEND);
		myDirectX->SetDepthBufferState(eDepthStencil::Z_ENABLED);
		myDirectX->SetRasterizer(eRasterizer::CULL_BACK);
#endif
	}

};