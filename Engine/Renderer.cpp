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

#include "imgui_impl_dx11.h"

#include <Brofiler.h>

#include <Input/InputHandle.h>
#include <Input/InputWrapper.h>

//#define AMBIENT_PASS_ONLY
namespace Hex
{
	bool Renderer::Initiate(Synchronizer* synchronizer, Camera* camera)
	{
		m_Engine = Engine::GetInstance();
		if (!m_Engine)
			return false;

		m_API = m_Engine->GetAPI();

		mySynchronizer = synchronizer;
		if (!mySynchronizer)
			return false;
		m_Camera = camera;
		if (!m_Camera)
			return false;

		myText = new CText("Data/Font/OpenSans-Bold.ttf", 8, 1);
		if (!myText)
			return false;

		myPointLight = new CPointLight; //Where should this live?
		myPointLight->Initiate();
		if (!myPointLight)
			return false;
		mySpotlight = new CSpotLight; // Where should this live?
		if (!mySpotlight)
			return false;

		m_Shadowlight = new ShadowSpotlight;
		m_Shadowlight->Initiate(
			CU::Vector3f(256.f, 128.f, 256.f)
			, CU::Vector3f(0.f, 0.f, 1.f)
			, 2048.f);

		m_DirectionalLightBuffer = m_API->CreateBuffer(sizeof(m_DirectionalLightStruct), nullptr, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE);
		m_DirectionalCamera = new Camera;
		m_DirectionalCamera->CreateOrthographicProjection(200.f, 200.f, 1.f, 128.f);

		m_DirectionalCamera->SetPosition({ 256.f, 128.f, 256.f });
		m_DirectionalCamera->RotateAroundX(CL::DegreeToRad(90.f) * 1.f);




		m_ShadowDepth = new Texture;
		m_ShadowDepth->InitiateAsRenderTarget(2048.f, 2048.f, "DirectionalLight : Depth Render Target");

		m_ShadowDepthStencil = new Texture;
		m_ShadowDepthStencil->InitiateAsDepthStencil(2048.f, 2048.f, "DirectionalLight : Depthstencil View");

		myDeferredRenderer = new DeferredRenderer; // Where should this live?
		if (!myDeferredRenderer->Initiate(m_ShadowDepthStencil))
			return false;

		m_Direction = { 0.f, -1.f, 0.f };


	

		myDepthTexture = new Texture; //Where should this live?
		DL_ASSERT_EXP(myDepthTexture, "DepthTexture in Renderer was null?");

		SWindowSize window_size;
		window_size = Engine::GetInstance()->GetWindowSize();

		myDepthTexture->Initiate(window_size.myWidth, window_size.myHeight
			, DEFAULT_USAGE | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL
			, DXGI_FORMAT_R32_TYPELESS
			, DXGI_FORMAT_R32_FLOAT
			, DXGI_FORMAT_D32_FLOAT
			, "Renderer : Depth");

		m_LightTexture = new Texture;
		m_LightTexture->InitiateAsRenderTarget(window_size.myWidth, window_size.myHeight, "LightTexture");


		mySkysphere = new SkySphere;
		mySkysphere->Initiate("Data/Model/Skysphere/SM_Skysphere.fbx", "Data/Shaders/T_Skysphere.json", m_Camera);
		mySkysphere->AddLayer("Data/Model/Skysphere/SM_Skysphere_Layer.fbx", "Data/Shaders/T_Skysphere_Layer.json");
		if (!mySkysphere)
			return false;

		mySprite = new Sprite;
		mySprite->Initiate("Data/Textures/colors.dds", CU::Vector2f(256.f, 256.f), CU::Vector2f(0.f, 0.f));

		myClearColor = new Sprite;
		myClearColor->Initiate("Data/Textures/blank.dds", CU::Vector2f(256.f, 256.f), CU::Vector2f(0.f, 0.f));



		my3DLine = new CLine3D; //Where should this live?
		if (!my3DLine)
			return false;

		my3DLine->Initiate();

		bool success = m_LightPass.Initiate(myDeferredRenderer->GetGBuffer(), m_Shadowlight->GetDepthStencil());
		DL_ASSERT_EXP(success, "failed to initiate lightpass!");

	/*	m_ParticleEmitter = new CEmitterInstance;
		m_ParticleEmitter->Initiate(mySynchronizer, myDepthTexture);*/



		m_ShadowEffect = m_Engine->GetEffect("Data/Shaders/T_Render_Depth.json");


		m_Engine->LoadModel("Data/Model/cube.fbx", "Data/Shaders/T_Deferred_Base.json");

		m_PostProcessManager.Initiate();
		m_PostProcessManager.SetPassesToProcess(PostProcessManager::HDR);
		return true;
	}

	bool Renderer::CleanUp()
	{
		m_LightPass.CleanUp();

		SAFE_DELETE(m_ShadowDepthStencil);
		SAFE_DELETE(m_ShadowDepth);

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
		
		SAFE_DELETE(myPointLight);
		SAFE_DELETE(mySpotlight);

		//m_ParticleEmitter->CleanUp();
		SAFE_DELETE(m_ParticleEmitter);
		
		return true;
	}

	void Renderer::Add2DCamera(Camera* aCamera)
	{
		my2DCamera = aCamera;
	}

#if defined (AMBIENT_PASS_ONLY)
	void Renderer::Render()
	{
		m_Engine->Clear();
		//myEngine->GetAPI()->ResetViewport();

		m_API->SetDepthBufferState(eDepthStencil::MASK_TEST);
		myDeferredRenderer->SetTargets();

		Render3DCommands();
		Texture::CopyData(myDepthTexture->GetDepthTexture(), myDeferredRenderer->GetDepthStencil()->GetDepthTexture());

		myDeferredRenderer->DeferredRender(myPrevFrame, m_Camera->GetPerspective());

		m_Engine->ResetRenderTargetAndDepth();
		myDeferredRenderer->Finalize();

		RenderLines();

		Render2DCommands();
		m_Engine->Present();

		mySynchronizer->WaitForLogic();
		mySynchronizer->SwapBuffer();
		mySynchronizer->RenderIsDone();
		myPrevFrame = m_Camera->GetOrientation();
	}
#else
	void Renderer::Render()
	{
		BROFILER_FRAME("Render");
		m_Engine->Clear();
		m_API->SetDepthStencilState(eDepthStencilState::MASK_TEST, 0);
		myDeferredRenderer->SetTargets(); 

		Render3DCommands();
		Texture::CopyData(myDepthTexture->GetDepthTexture(), myDeferredRenderer->GetDepthStencil()->GetDepthTexture());
		ProcessShadows();
		ProcessShadows(m_DirectionalCamera);
		myDeferredRenderer->DeferredRender(myPrevFrame, m_Camera->GetPerspective(), CU::Math::Inverse(m_DirectionalCamera->GetOrientation()) * m_DirectionalCamera->GetPerspective() , m_Direction);

		/* This has to be moved or removed */
		InputHandle* input_handle = Engine::GetInstance()->GetInputHandle();
		if (input_handle->GetInputWrapper()->IsDown(KButton::Y))
		{
			CU::Vector3f target_position ( 256.f, 64.f, 256.f );
			m_DirectionalCamera->RotateAroundPoint(target_position);

			m_Direction = target_position - m_DirectionalCamera->GetPosition();
			CU::Math::Normalize(m_Direction);
		}

		/* Should be moved into settings */

		if (input_handle->GetInputWrapper()->OnDown(KButton::NUMMINUS))
		{
			float current_fov = m_Camera->GetFOV();
			current_fov -= 1.f;
			m_Camera->SetFOV(current_fov);
		}

		if (input_handle->GetInputWrapper()->OnDown(KButton::NUMADD))
		{
			float current_fov = m_Camera->GetFOV();
			current_fov += 1.f;
			m_Camera->SetFOV(current_fov);
		}

		RenderPointlight();
		RenderSpotlight();


		m_Engine->ResetRenderTargetAndDepth();

	/*	m_ToneMapping->Activate();

		m_API->DisableZBuffer();
		
		m_API->GetContext()->DrawIndexed(6, 0, 0);

		m_API->EnableZBuffer();

		
		m_ToneMapping->Deactivate();*/

		myDeferredRenderer->SetBuffers();
		m_PostProcessManager.Process(myDeferredRenderer->GetFinalTexture());
		//myDeferredRenderer->Finalize(m_LightTexture);




		//mySkysphere->Update(Engine::GetInstance()->GetDeltaTime());
		//mySkysphere->Render(myPrevFrame, myDepthTexture);
		//RenderParticles();

		

		// POST PROCESSING




		//RenderLines();

		Render2DCommands();

		ImGui::Render();
		m_Engine->Present();

		//mySynchronizer->AddRenderCommand(RenderCommand(eType::SPRITE, m_Shadowlight->GetDepthStencil()->GetDepthStencilView(), CU::Vector2f(1920.f - 128.f, 128.f)));
		//mySynchronizer->AddRenderCommand(RenderCommand(eType::SPRITE, m_ShadowDepthStencil->GetDepthStencilView(), CU::Vector2f(1920.f - 128.f, 128.f + 256.f)));
		//mySynchronizer->AddRenderCommand(RenderCommand(eType::MODEL, "Data/Model/cube.fbx", m_DirectionalCamera->GetOrientation(), CU::Vector4f(1, 1, 1, 1)));


		mySynchronizer->WaitForLogic();
		mySynchronizer->SwapBuffer();
		mySynchronizer->RenderIsDone();
		myPrevFrame = m_Camera->GetOrientation();
		m_Engine->ToggleFrame();
	}
#endif

	void Renderer::AddTerrain(CTerrain* someTerrain)
	{
		myTerrainArray.Add(someTerrain);
	}

	void Renderer::Render3DCommands()
	{
		const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::e3D);

		//m_API->SetRasterizer(m_RenderWireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_BACK);
		m_API->SetBlendState(eBlendStates::BLEND_FALSE);
		//m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
		for (CTerrain* terrain : myTerrainArray)
		{
			if (!terrain->HasLoaded())
				continue;

			if (m_ProcessDirectionalShadows)
			{
				terrain->Render(m_DirectionalFrame, m_Camera->GetPerspective(), CU::Vector4f(1, 1, 1, 1), true);

			}
			else if (m_ProcessShadows)
			{
				terrain->Render(myPrevShadowFrame, m_Camera->GetPerspective(), CU::Vector4f(1, 1, 1, 1), true);

			}
			else
			{
				terrain->Render(myPrevFrame, m_Camera->GetPerspective(), CU::Vector4f(1, 1, 1, 1), false);

			}

			//terrain->Render(m_ProcessShadows ? myPrevShadowFrame : myPrevFrame, m_Camera->GetPerspective(), CU::Vector4f(1,1,1,1), m_ProcessShadows);
		}

		for (const RenderCommand& command : commands)
		{
			switch (command.myType)
			{
				case eType::MODEL:
				{
					if (command.m_KeyOrText.empty())
					{
						TRACE_LOG("Key was empty");
						continue;
					}

					
					m_API->SetRasterizer(m_RenderWireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_NONE);
					m_API->SetBlendState(eBlendStates::BLEND_FALSE);
										 
					CModel* model = m_Engine->GetModel(command.m_KeyOrText);
					model->SetOrientation(command.m_Orientation);


					if (m_ProcessDirectionalShadows)
					{
						model->Render(m_DirectionalFrame, m_Camera->GetPerspective(), command.m_Scale, true);

					}
					else if (m_ProcessShadows)
					{
						model->Render(myPrevShadowFrame, m_Camera->GetPerspective(), command.m_Scale, true);

					}
					else
					{
						model->Render(myPrevFrame, m_Camera->GetPerspective(), command.m_Scale, false);

					}
				}break;
			}
		}
	}

	void Renderer::Render2DCommands()
	{
		const CU::GrowingArray<RenderCommand>& commands2D = mySynchronizer->GetRenderCommands(eCommandBuffer::e2D);
		m_API->SetRasterizer(eRasterizer::CULL_NONE);
		m_API->SetDepthStencilState(eDepthStencilState::Z_DISABLED, 1);
		for each(const RenderCommand& command in commands2D)
		{
			switch (command.myType)
			{
				case eType::TEXT:
				{
					myText->SetText(command.m_KeyOrText);
					myText->SetPosition({ command.myPosition.x, command.myPosition.y });
					myText->Render(m_Camera);
				}break;
				case eType::SPRITE: 
				{

					myClearColor->SetPosition({ command.myPosition.x, command.myPosition.y });
					myClearColor->Render(m_Camera);

					mySprite->SetPosition({ command.myPosition.x, command.myPosition.y });
					mySprite->SetShaderView(command.m_ShaderResource);
					mySprite->Render(m_Camera);
				}break;
			};
		}
		m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
		m_API->SetRasterizer(eRasterizer::CULL_BACK);
	}

	void Renderer::RenderSpotlight()
	{
		const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::eSpotlight);
		m_API->SetRasterizer(eRasterizer::CULL_NONE);
		m_API->SetDepthStencilState(eDepthStencilState::READ_NO_WRITE, 0);
		Effect* effect = m_LightPass.GetSpotlightEffect()	;
		effect->Activate();

		for each(const RenderCommand& command in commands)
		{
			DL_ASSERT_EXP(command.myType == eType::SPOTLIGHT, "Wrong command type in spotlight buffer.");
			m_API->SetBlendState(eBlendStates::LIGHT_BLEND);

			mySpotlight->SetPosition(command.myPosition);
			mySpotlight->SetRange(command.myRange);
			mySpotlight->SetColor(CU::Vector4f(command.myColor.x, command.myColor.y, command.myColor.z, 1));
			mySpotlight->SetAngle(command.myAngle);
			//mySpotlight->set
			mySpotlight->DoTranslation(command.m_Orientation);

			m_LightPass.RenderSpotlight(mySpotlight, m_Camera, myPrevFrame, m_Shadowlight->GetMVP());
		}
		effect->Deactivate();
		m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
		m_API->SetRasterizer(eRasterizer::CULL_BACK);
	}

	void Renderer::RenderPointlight()
	{
		const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::ePointlight);

		m_API->SetRasterizer(eRasterizer::CULL_NONE);
		m_API->SetDepthStencilState(eDepthStencilState::READ_NO_WRITE, 0);
		Effect* effect = m_LightPass.GetPointlightEffect();
		effect->Activate();

		for each(const RenderCommand& command in commands)
		{
			DL_ASSERT_EXP(command.myType == eType::POINTLIGHT, "Wrong command type in pointlight buffer.");
			m_API->SetBlendState(eBlendStates::LIGHT_BLEND);
			myPointLight->SetPosition(command.myPosition);
			myPointLight->SetRange(command.myRange);
			myPointLight->SetColor(CU::Vector4f(command.myColor.x, command.myColor.y, command.myColor.z, 1));
			myPointLight->Update();
			m_LightPass.RenderPointlight(myPointLight, m_Camera, myPrevFrame, m_Shadowlight->GetMVP());
		}

		effect->Deactivate();

		m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
		m_API->SetRasterizer(eRasterizer::CULL_BACK);
	}

	void Renderer::RenderParticles()
	{
		m_API->SetBlendState(eBlendStates::ALPHA_BLEND);
		m_API->SetRasterizer(eRasterizer::CULL_NONE);

		if(m_ProcessShadows)
			m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
		else
			m_API->SetDepthStencilState(eDepthStencilState::READ_NO_WRITE, 0);

		const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::e3D);
		for (const RenderCommand& command : commands)
		{
			switch (command.myType)
			{
				case eType::PARTICLE:
				{
					if (!m_ProcessShadows)
					{
						m_ParticleEmitter->SetPosition(CU::Vector3f(100.f, 0.f, 31.f));
						m_ParticleEmitter->Update(m_Engine->GetDeltaTime());
					}
					m_ParticleEmitter->Render(m_ProcessShadows ? myPrevShadowFrame : myPrevFrame, m_Camera->GetPerspective());
				}break;
			}
		}
		m_API->SetRasterizer(eRasterizer::CULL_BACK);
	}

	void Renderer::RenderLines()
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
					m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
					my3DLine->Update(command.firstPoint, command.secondPoint);
					my3DLine->Render(myPrevFrame, m_Camera->GetPerspective());
				}break;
				case eType::LINE_Z_DISABLE:
				{
					m_API->SetDepthStencilState(eDepthStencilState::Z_DISABLED, 1);
					my3DLine->Update(command.firstPoint, command.secondPoint);
					my3DLine->Render(myPrevFrame, m_Camera->GetPerspective());
				}break;
			}
		}

		m_API->SetBlendState(eBlendStates::NO_BLEND);
		m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
		m_API->SetRasterizer(eRasterizer::CULL_BACK);
	}

	void Renderer::ProcessShadows()
	{
		m_API->SetDepthStencilState(eDepthStencilState::MASK_TEST, 0);
		m_ProcessShadows = true;

		Camera* camera = m_Camera;
		m_Camera = m_Shadowlight->GetCamera();
		m_Shadowlight->SetViewport();

		m_Shadowlight->ClearTexture();
		m_Shadowlight->SetTargets();
		m_Shadowlight->ToggleShader(m_ProcessShadows);
		Render3DCommands();
		//RenderParticles();
		m_Shadowlight->Copy();
		m_Engine->ResetRenderTargetAndDepth();
		m_API->ResetViewport();
		
		myPrevShadowFrame = m_Camera->GetOrientation();
		m_Camera = camera;

		m_ProcessShadows = false;
		m_Shadowlight->ToggleShader(m_ProcessShadows);
	}

	void Renderer::ProcessShadows(Camera* camera)
	{
		m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
		IDevContext* ctx = m_API->GetContext();
		Camera* old_camera = m_Camera;
		m_Camera = camera;
		m_Shadowlight->SetViewport();

		float clear[4] = { 0.f, 0.f, 0.f, 0.f };
		ctx->ClearRenderTargetView(m_ShadowDepth->GetRenderTargetView(), clear);
		ctx->ClearDepthStencilView(m_ShadowDepthStencil->GetDepthView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		ctx->OMSetRenderTargets(1, m_ShadowDepth->GetRenderTargetRef(), m_ShadowDepthStencil->GetDepthView());

		m_API->SetVertexShader(m_ShadowEffect->GetVertexShader()->m_Shader);
		m_API->SetPixelShader(m_ShadowEffect->GetPixelShader()->m_Shader);

		m_ShadowEffect->Activate();
		
		m_ProcessDirectionalShadows = true;
		Render3DCommands();
		m_ProcessDirectionalShadows = false;

		m_Engine->ResetRenderTargetAndDepth();
		m_API->ResetViewport();

		m_DirectionalFrame = m_Camera->GetOrientation();
		m_Camera = old_camera; 

		m_ShadowEffect->Deactivate();
	}

	void Renderer::ToggleWireframe()
	{
		m_RenderWireframe = !m_RenderWireframe;
	}
};


