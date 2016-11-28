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
#include "ShadowSpotlight.h"
#include "GBuffer.h"
#include "Texture.h"
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

		myPointLight = new CPointLight; //Where should this live?
		myPointLight->Initiate();
		if (!myPointLight)
			return false;
		//mySpotlight = new CSpotLight; // Where should this live?
		//if (!mySpotlight)
		//	return false;

		m_Shadowlight = new ShadowSpotlight;
		m_Shadowlight->Initiate(
			CU::Vector3f(95, 7.f, 28.f)
			, CU::Vector3f(1.f, 0.5f, 0.f)
			, 2048.f);

		myDeferredRenderer = new CDeferredRenderer; // Where should this live?
		if (!myDeferredRenderer->Initiate(m_Shadowlight->m_Depth))
			return false;

		myDepthTexture = new CTexture; //Where should this live?
		if (!myDepthTexture)
			return false;
		myDepthTexture->Initiate(CEngine::GetInstance()->GetWindowSize().myWidth, CEngine::GetInstance()->GetWindowSize().myHeight
			, DEFAULT_USAGE | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL
			, DXGI_FORMAT_R32_TYPELESS
			, DXGI_FORMAT_R32_FLOAT
			, DXGI_FORMAT_D32_FLOAT
			, "Renderer : Depth");

		mySkysphere = new CSkySphere;
		mySkysphere->Initiate("Data/Model/Skysphere/SM_Skysphere.fbx", "Data/Shaders/T_Skysphere.json", camera_3d);
		mySkysphere->AddLayer("Data/Model/Skysphere/SM_Skysphere_Layer.fbx", "Data/Shaders/T_Skysphere_Layer.json");
		if (!mySkysphere)
			return false;

	

		mySprite = new CSprite;
		mySprite->Initiate("Data/Textures/colors.dds", CU::Vector2f(256.f, 256.f), CU::Vector2f(0.f, 0.f));
		myClearColor = new CSprite;
		myClearColor->Initiate("Data/Textures/flat_height.dds", CU::Vector2f(256.f, 256.f), CU::Vector2f(0.f, 0.f));
		myEngine = CEngine::GetInstance();
		if (!myEngine)
			return false;
#ifdef SNOWBLIND_DX11
		m_API = myEngine->GetAPI();
#endif
		my3DLine = new CLine3D; //Where should this live?
		if (!my3DLine)
			return false;

		my3DLine->Initiate();

		DL_ASSERT_EXP(m_LightPass.Initiate(myDeferredRenderer->GetGBuffer()), "failed to initiate LightPass!");

		myEngine->LoadModel("Data/Model/cube.fbx", "Data/Shaders/T_Deferred_Base.json");
		return true;
	}

	bool CRenderer::CleanUp()
	{
		m_LightPass.CleanUp();

		m_Shadowlight->CleanUp();
		SAFE_DELETE(m_Shadowlight);

		SAFE_DELETE(my3DLine);
		SAFE_DELETE(mySprite);
		SAFE_DELETE(myClearColor);

		mySkysphere->CleanUp();
		SAFE_DELETE(mySkysphere);

		myDepthTexture->CleanUp();
		SAFE_DELETE(myDepthTexture);
		SAFE_DELETE(my2DCamera);

		myDeferredRenderer->CleanUp();
		SAFE_DELETE(myDeferredRenderer);

		SAFE_DELETE(myText);

		/*for (CTerrain* terrain : myTerrainArray)
		{
			if (terrain)
			{
				terrain->CleanUp();
				SAFE_DELETE(terrain);
			}
		}*/
				
		SAFE_DELETE(myPointLight);

		SAFE_DELETE(mySpotlight);
		
		
		
		return true;
	}

	void CRenderer::Add2DCamera(CCamera* aCamera)
	{
		my2DCamera = aCamera;
	}

	void CRenderer::Render()
	{
		myEngine->Clear();
		//myEngine->GetAPI()->ResetViewport();

		ProcessShadows();
		m_API->SetDepthBufferState(eDepthStencil::MASK_TEST);
		myDeferredRenderer->SetTargets();
		Render3DCommands();
		
		CTexture::CopyData(myDepthTexture->GetDepthTexture(), myDeferredRenderer->GetDepthStencil()->GetDepthTexture());
		myDeferredRenderer->DeferredRender(myPrevFrame, myCamera->GetProjection(), m_Shadowlight->GetCamera()->GetProjection(), myPrevShadowFrame);

		//RenderPointlight();
		//RenderSpotlight();

		myEngine->ResetRenderTargetAndDepth();
		mySkysphere->Update(CEngine::GetInstance()->GetDeltaTime());
		myDeferredRenderer->Finalize();
		mySkysphere->Render(myPrevFrame, myDepthTexture);

		//RenderParticles();
		RenderLines();

		Render2DCommands();
		myEngine->Present();

		mySynchronizer->WaitForLogic();
		mySynchronizer->SwapBuffer();
		mySynchronizer->RenderIsDone();
		myPrevFrame = myCamera->GetOrientation();


		//Debug stuff
		/*mySynchronizer->AddRenderCommand(RenderCommand(eType::SPRITE, myDeferredRenderer->GetGBuffer()->myAlbedo->GetShaderView(), CU::Vector2f(1920.f - 128.f, 128.f)));
		mySynchronizer->AddRenderCommand(RenderCommand(eType::SPRITE, myDeferredRenderer->GetGBuffer()->myNormal->GetShaderView(), CU::Vector2f(1920.f - 128.f, 384.f)));
		mySynchronizer->AddRenderCommand(RenderCommand(eType::SPRITE, myDeferredRenderer->GetGBuffer()->myDepth->GetShaderView(), CU::Vector2f(1920.f - 128.f, 640.f)));*/

		mySynchronizer->AddRenderCommand(RenderCommand(eType::SPRITE, m_Shadowlight->GetRenderTarget()->GetShaderView(), CU::Vector2f(1920.f - 128.f, 128.f)));
		mySynchronizer->AddRenderCommand(RenderCommand(eType::SPRITE, m_Shadowlight->m_Normal->GetShaderView(), CU::Vector2f(1920.f - 128.f, 384.f)));
		mySynchronizer->AddRenderCommand(RenderCommand(eType::SPRITE, m_Shadowlight->m_Depth->GetShaderView(), CU::Vector2f(1920.f - 128.f, 640.f)));
		//mySynchronizer->AddRenderCommand(RenderCommand(eType::SPRITE, m_Shadowlight->m_Texture->GetDepthStencilView(), CU::Vector2f(1920.f - 384.f, 896.f)));

		mySynchronizer->AddRenderCommand(RenderCommand(eType::MODEL, "Data/Model/cube.fbx", m_Shadowlight->GetOrientation()));
	}

	void CRenderer::AddTerrain(CTerrain* someTerrain)
	{
		myTerrainArray.Add(someTerrain);
	}

	void CRenderer::Render3DCommands()
	{
		const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::e3D);
		for (const RenderCommand& command : commands)
		{
			switch (command.myType)
			{
				case eType::MODEL:
				{
					if (command.m_KeyOrText.empty())
					{
						TRACE_LOG("Failed to find model!");
						continue;
					}

					m_API->SetRasterizer(m_RenderWireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_BACK);
					m_API->SetBlendState(eBlendStates::BLEND_FALSE);
					 
					CModel* model = myEngine->GetModel(command.m_KeyOrText);
					model->SetOrientation(command.m_Orientation);
					model->Render( m_ProcessShadows ? myPrevShadowFrame : myPrevFrame, myCamera->GetProjection());
				}break;
				case eType::SKYSPHERE:
				{
					if(m_ProcessShadows)
						continue;
					mySkysphere->SetPosition(myPrevFrame.GetPosition());
				}break;
				case eType::TERRAIN:
				{
					m_API->SetRasterizer(m_RenderWireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_BACK);
					m_API->SetBlendState(eBlendStates::BLEND_FALSE);
					for (CTerrain* terrain : myTerrainArray)
					{
						if(!terrain->HasLoaded())
							continue;
						terrain->Render(m_ProcessShadows ? myPrevShadowFrame : myPrevFrame, myCamera->GetProjection());
					}
				}break;
			}
		}
	}

	void CRenderer::Render2DCommands()
	{
		const CU::GrowingArray<RenderCommand>& commands2D = mySynchronizer->GetRenderCommands(eCommandBuffer::e2D);
		m_API->SetRasterizer(eRasterizer::CULL_NONE);
		m_API->SetDepthBufferState(eDepthStencil::Z_DISABLED);
		for each(const RenderCommand& command in commands2D)
		{
			switch (command.myType)
			{
				case eType::TEXT:
				{
					myText->SetText(command.m_KeyOrText);
					myText->SetPosition({ command.myPosition.x, command.myPosition.y });
					myText->Render(my2DCamera);
				}break;
				case eType::SPRITE:
				{

					myClearColor->SetPosition({ command.myPosition.x, command.myPosition.y });
					myClearColor->Render(my2DCamera);

					mySprite->SetPosition({ command.myPosition.x, command.myPosition.y });
					mySprite->SetShaderView(command.m_ShaderResource);
					mySprite->Render(my2DCamera);
				}break;
			};
		}
		m_API->SetDepthBufferState(eDepthStencil::Z_ENABLED);
		m_API->SetRasterizer(eRasterizer::CULL_BACK);
	}

	void CRenderer::RenderSpotlight()
	{
		const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::eSpotlight);
		m_API->SetRasterizer(eRasterizer::CULL_NONE);
		m_API->SetDepthBufferState(eDepthStencil::READ_NO_WRITE);
		CEffect* effect = m_LightPass.GetSpotlightEffect()	;
		effect->Activate();

		for each(const RenderCommand& command in commands)
		{
			DL_ASSERT_EXP(command.myType == eType::SPOTLIGHT, "Wrong command type in spotlight buffer.");
			m_API->SetBlendState(eBlendStates::LIGHT_BLEND);

			mySpotlight->SetPosition(command.myPosition);
			mySpotlight->SetRange(command.myRange);
			mySpotlight->SetColor(CU::Vector4f(command.myColor.r, command.myColor.g, command.myColor.b, 1));
			mySpotlight->SetAngle(command.myAngle);

			mySpotlight->DoTranslation(command.m_Orientation);

			m_LightPass.RenderSpotlight(mySpotlight, myCamera, myPrevFrame);
		}
		effect->Deactivate();
		m_API->SetDepthBufferState(eDepthStencil::Z_ENABLED);
		m_API->SetRasterizer(eRasterizer::CULL_BACK);
	}

	void CRenderer::RenderPointlight()
	{
		const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::ePointlight);

		m_API->SetRasterizer(eRasterizer::CULL_NONE);
		m_API->SetDepthBufferState(eDepthStencil::READ_NO_WRITE);
		CEffect* effect = m_LightPass.GetPointlightEffect();
		effect->Activate();

		for each(const RenderCommand& command in commands)
		{
			DL_ASSERT_EXP(command.myType == eType::POINTLIGHT, "Wrong command type in pointlight buffer.");
			m_API->SetBlendState(eBlendStates::LIGHT_BLEND);
			myPointLight->SetPosition(command.myPosition);
			myPointLight->SetRange(command.myRange);
			myPointLight->SetColor(CU::Vector4f(command.myColor.r, command.myColor.g, command.myColor.b, 1));
			myPointLight->Update();
			m_LightPass.RenderPointlight(myPointLight, myCamera, myPrevFrame);
		}
		effect->Deactivate();

		m_API->SetDepthBufferState(eDepthStencil::Z_ENABLED);
		m_API->SetRasterizer(eRasterizer::CULL_BACK);
	}

	void CRenderer::RenderParticles()
	{
		m_API->SetBlendState(eBlendStates::ALPHA_BLEND);
		m_API->SetRasterizer(eRasterizer::CULL_NONE);
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
		m_API->SetRasterizer(eRasterizer::CULL_BACK);
	}

	void CRenderer::RenderLines()
	{
		m_API->SetBlendState(eBlendStates::NO_BLEND);
		m_API->SetRasterizer(eRasterizer::CULL_NONE);

		ID3D11RenderTargetView* backbuffer = m_API->GetBackbuffer();
		ID3D11DepthStencilView* depth = myDeferredRenderer->GetDepthStencil()->GetDepthView();
		m_API->GetContext()->OMSetRenderTargets(1, &backbuffer, depth);

		const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::eLine);
		for each(const RenderCommand& command in commands)
		{
			switch (command.myType)
			{
				case eType::LINE_Z_ENABLE:
				{
					m_API->SetDepthBufferState(eDepthStencil::Z_ENABLED);
					my3DLine->Update(command.firstPoint, command.secondPoint);
					my3DLine->Render(myPrevFrame, myCamera->GetProjection());
				}break;
				case eType::LINE_Z_DISABLE:
				{
					m_API->SetDepthBufferState(eDepthStencil::Z_DISABLED);
					my3DLine->Update(command.firstPoint, command.secondPoint);
					my3DLine->Render(myPrevFrame, myCamera->GetProjection());
				}break;
			}
		}
		m_API->SetBlendState(eBlendStates::NO_BLEND);
		m_API->SetDepthBufferState(eDepthStencil::Z_ENABLED);
		m_API->SetRasterizer(eRasterizer::CULL_BACK);
	}

	void CRenderer::ProcessShadows()
	{
		CCamera* camera = myCamera;
		m_ProcessShadows = true;
		m_Shadowlight->SetViewport();

		m_Shadowlight->ClearTexture();

		m_Shadowlight->SetTargets();


		m_API->SetDepthBufferState(eDepthStencil::MASK_TEST);
		myCamera = m_Shadowlight->GetCamera();
		Render3DCommands();

		myEngine->ResetRenderTargetAndDepth();
		m_API->ResetViewport();
		
		myPrevShadowFrame = myCamera->GetOrientation();
		myCamera = camera;
		m_ProcessShadows = false;
	}

	void CRenderer::ToggleWireframe()
	{
		m_RenderWireframe = !m_RenderWireframe;
	}
};