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

#include "Terrain.h"

#include "Line3D.h"
#include "LightPass.h"
#include "ShadowSpotlight.h"
#include "GBuffer.h"
#include "Texture.h"

#include "imgui_impl_dx11.h"

#ifdef _PROFILE
#include <easy/profiler.h>
#endif 

#include <Input/InputHandle.h>
#include <Input/InputWrapper.h>

bool Renderer::Initiate(Synchronizer* synchronizer, Camera* camera)
{
	m_Engine = Engine::GetInstance();
	m_API = m_Engine->GetAPI();
	mySynchronizer = synchronizer;
	m_Camera = camera;

	WindowSize window_size;
	window_size = Engine::GetInstance()->GetWindowSize();

	myText = new CText("Data/Font/OpenSans-Bold.ttf", 8, 1);

	myPointLight = new PointLight; //Where should this live?
	mySpotlight = new SpotLight; // Where should this live?
	mySpotlight->Initiate();

	myDeferredRenderer = new DeferredRenderer; // Where should this live?
	if ( !myDeferredRenderer->Initiate(0) )
		return false;

	myDepthTexture = new Texture; //Where should this live?
	DL_ASSERT_EXP(myDepthTexture, "DepthTexture in Renderer was null?");

	myDepthTexture->Initiate(window_size.m_Width, window_size.m_Height
		, DEFAULT_USAGE | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL
		, DXGI_FORMAT_R32_TYPELESS
		, DXGI_FORMAT_R32_FLOAT
		, DXGI_FORMAT_D32_FLOAT
		, "Renderer : Depth");

	mySprite = new Sprite;
	mySprite->Initiate("Data/Textures/colors.dds", CU::Vector2f(128.f, 128.f), CU::Vector2f(0.f, 0.f));

	myClearColor = new Sprite;
	myClearColor->Initiate("Data/Textures/blank.dds", CU::Vector2f(128.f, 128.f), CU::Vector2f(0.f, 0.f));

	my3DLine = new CLine3D; //Where should this live?
	my3DLine->Initiate();

	bool success = m_LightPass.Initiate(myDeferredRenderer->GetGBuffer(), 0);
	DL_ASSERT_EXP(success, "failed to initiate lightpass!");

	m_ParticleEmitter = new CEmitterInstance;
	m_ParticleEmitter->Initiate(mySynchronizer, myDepthTexture);

	m_Atmosphere.Initiate(724.f, 728.f, { 512.f, 0.f, 512.f });


	m_PostProcessManager.Initiate();
	m_PostProcessManager.SetPassesToProcess(PostProcessManager::HDR);

	m_RenderContext.m_API = Engine::GetAPI();
	m_RenderContext.m_Device = Engine::GetAPI()->GetDevice();
	m_RenderContext.m_Context = Engine::GetAPI()->GetContext();
	m_RenderContext.m_Engine = Engine::GetInstance();

	m_ShadowPass.Initiate(this);

	m_DirectionalShadow.Initiate(2048.f);

	return true;
}

bool Renderer::CleanUp()
{
	m_LightPass.CleanUp();
	m_ShadowPass.CleanUp();
	m_DirectionalShadow.CleanUp();
	m_PostProcessManager.CleanUp();

	SAFE_DELETE(my3DLine);
	SAFE_DELETE(mySprite);
	SAFE_DELETE(myClearColor);

	myDepthTexture->CleanUp();
	SAFE_DELETE(myDepthTexture);

	myDeferredRenderer->CleanUp();
	SAFE_DELETE(myDeferredRenderer);

	SAFE_DELETE(myText);

	SAFE_DELETE(myPointLight);

	mySpotlight->CleanUp();
	SAFE_DELETE(mySpotlight);

	m_ParticleEmitter->CleanUp();
	SAFE_DELETE(m_ParticleEmitter);

	m_Atmosphere.CleanUp();

	return true;
}

void Renderer::Render()
{
#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Magenta);
#endif
	m_Engine->Clear();
	myDeferredRenderer->SetGBufferAsTarget(m_RenderContext);

	Render3DCommands();

	Texture::CopyData(myDepthTexture->GetDepthTexture(), myDeferredRenderer->GetDepthStencil()->GetDepthTexture());

	m_ShadowPass.ProcessShadows(&m_DirectionalShadow, m_RenderContext);

	myDeferredRenderer->DeferredRender( 
		m_Camera->GetOrientation(), 
		m_Camera->GetPerspective(), 
		m_DirectionalShadow.GetMVP(),
		m_Direction,
		m_RenderContext);

	/* This has to be moved or removed */
	//InputHandle* input_handle = Engine::GetInstance()->GetInputHandle();
	//if ( input_handle->GetInputWrapper()->IsDown(KButton::Y) )
	//{
	//	CU::Vector3f target_position(512, 512.f, 512.f);
	//	m_DirectionalCamera->RotateAroundPoint(target_position);

	//	m_Direction = target_position - m_DirectionalCamera->GetPosition();
	//	CU::Math::Normalize(m_Direction);
	//}

	RenderPointlight();
	RenderSpotlight();


	m_Engine->ResetRenderTargetAndDepth();

	myDeferredRenderer->SetBuffers(m_RenderContext); //This is just the quad
	m_PostProcessManager.Process(myDeferredRenderer->GetFinalTexture());

	if ( m_PostProcessManager.GetFlags() == 0 )
		myDeferredRenderer->Finalize(m_RenderContext);


	//m_Atmosphere.SetLightData(m_Direction, m_DirectionalCamera->GetPosition());
	m_Atmosphere.Render(m_Camera->GetOrientation(), myDeferredRenderer->GetDepthStencil(), m_RenderContext);

	m_API->GetContext()->OMSetRenderTargets(1, m_API->GetBackbufferRef(), myDeferredRenderer->GetDepthStencil()->GetDepthView());
	RenderParticles();

	m_Engine->ResetRenderTargetAndDepth();

	RenderNonDeferred3DCommands();

	RenderLines();
	Render2DCommands();

	//m_ShadowPass.ProcessShadows(m_DirectionalCamera, m_RenderContext); //Directional Light Shadow 

	ImGui::Render();

	m_Engine->Present();

//#ifdef _DEBUG
//	SLinePoint points[2];
//	points[0].position = CU::Vector4f(m_DirectionalCamera->GetPosition());
//	points[0].color = CU::Vector4f(255.f, 0.f, 0.f, 255.f);
//	points[1].position = CU::Vector4f(512.f, 0.f, 512.f, 1.f);
//	points[1].color = CU::Vector4f(255.f, 0.f, 0.f, 255.f);
//
//	mySynchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, points[0], points[1]));
//#endif

	mySynchronizer->WaitForLogic();
	mySynchronizer->SwapBuffer();
	mySynchronizer->RenderIsDone();

}

void Renderer::AddTerrain(CTerrain* someTerrain)
{
	myTerrainArray.Add(someTerrain);
}

void Renderer::RenderNonDeferred3DCommands()
{
#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Amber);
#endif

	m_API->EnableZBuffer();
	const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::e3D);

	for ( const RenderCommand& command : commands )
	{
		switch ( command.myType )
		{
			case eType::MODEL_NO_DEFERRED:
			{
				if ( command.m_KeyOrText.empty() )
				{
					TRACE_LOG("Key was empty");
					continue;
				}


				m_API->SetRasterizer(command.m_Wireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_BACK);
				m_API->SetBlendState(eBlendStates::BLEND_FALSE);

				Model* model = m_Engine->GetModel(command.m_KeyOrText);
				model->SetOrientation(command.m_Orientation);
				model->Render(m_Camera->GetOrientation(), m_Camera->GetPerspective(), m_RenderContext);
			}break;
		}
	}
}

void Renderer::Render3DCommands()
{
	const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::e3D);

	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_API->SetBlendState(eBlendStates::BLEND_FALSE);

	for (CTerrain* terrain : myTerrainArray)
	{
		if (!terrain->HasLoaded())
			continue;
	
		terrain->Render(m_Camera->GetOrientation(), m_Camera->GetPerspective(), m_RenderContext);
	}

	for ( const RenderCommand& command : commands )
	{
		switch ( command.myType )
		{
			case eType::MODEL:
			{
				if ( command.m_KeyOrText.empty() )
				{
					TRACE_LOG("Key was empty");
					continue;
				}

				m_API->SetBlendState(eBlendStates::BLEND_FALSE);
				Model* model = m_Engine->GetModel(command.m_KeyOrText);
				model->SetOrientation(command.m_Orientation);
				m_API->SetRasterizer(command.m_Wireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_BACK);
				model->Render(m_Camera->GetOrientation(), m_Camera->GetPerspective(), m_RenderContext);

			} break;
		}
	}
}

void Renderer::Render3DShadows(const CU::Matrix44f& orientation, Camera* camera)
{
	const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::e3D);
	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_API->SetBlendState(eBlendStates::BLEND_FALSE);
	m_API->SetRasterizer(eRasterizer::CULL_NONE);

	for ( const RenderCommand& command : commands )
	{
		Model* model = m_Engine->GetModel(command.m_KeyOrText);
		model->SetOrientation(command.m_Orientation);
		model->ShadowRender(orientation, camera->GetPerspective(), m_RenderContext);
	}
}

void Renderer::Render2DCommands()
{
	const CU::GrowingArray<RenderCommand>& commands2D = mySynchronizer->GetRenderCommands(eCommandBuffer::e2D);
	m_API->SetRasterizer(eRasterizer::CULL_NONE);
	m_API->SetDepthStencilState(eDepthStencilState::Z_DISABLED, 0);
	m_API->SetBlendState(eBlendStates::NO_BLEND);
	for ( const RenderCommand& command : commands2D )
	{
		switch ( command.myType )
		{
			case eType::TEXT:
			{
				myText->SetText(command.m_KeyOrText);
				myText->SetPosition({ command.myPosition.x, command.myPosition.y });
				myText->Render(m_Camera);
			} break;
			case eType::SPRITE:
			{
				mySprite->SetPosition({ command.myPosition.x, command.myPosition.y });
				mySprite->SetShaderView(command.m_ShaderResource);
				mySprite->Render(m_Camera);
			} break;
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
	Effect* effect = m_LightPass.GetSpotlightEffect();
	//effect->Activate();

	SpotlightData data;
	for ( const RenderCommand& command : commands )
	{
		DL_ASSERT_EXP(command.myType == eType::SPOTLIGHT, "Wrong command type in spotlight buffer.");

		data.myAngle = command.myAngle;
		data.myRange = command.myRange;
		data.myLightColor = command.myColor;
		data.myLightPosition = command.myPosition;
		data.myOrientation = command.m_Orientation;

		mySpotlight->SetData(data);
		CU::Matrix44f shadow_mvp;
		
		if ( command.m_ShadowCasting )
		{
			ShadowSpotlight* shadow = mySpotlight->GetShadowSpotlight();
			m_ShadowPass.ProcessShadows(shadow, m_RenderContext);
			shadow_mvp = shadow->GetMVP();
			Effect* effect = Engine::GetInstance()->GetEffect("Shaders/T_Deferred_Spotlight.json");
			effect->AddShaderResource(shadow->GetDepthStencil(), Effect::SHADOWMAP);
			myDeferredRenderer->SetRenderTarget(m_RenderContext);
		}

		effect->Use();
		m_LightPass.RenderSpotlight(mySpotlight, m_Camera, m_Camera->GetOrientation(), shadow_mvp, m_RenderContext);

	}
	effect->Clear();

	//effect->Deactivate();
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

	for ( const RenderCommand& command : commands )
	{
		DL_ASSERT_EXP(command.myType == eType::POINTLIGHT, "Wrong command type in pointlight buffer.");
		m_API->SetBlendState(eBlendStates::LIGHT_BLEND);
		myPointLight->SetPosition(command.myPosition);
		myPointLight->SetRange(command.myRange);
		myPointLight->SetColor(CU::Vector4f(command.myColor.x, command.myColor.y, command.myColor.z, 1));
		myPointLight->Update();
		//m_LightPass.RenderPointlight(myPointLight, m_Camera, myPrevFrame, m_Shadowlight->GetMVP(), m_RenderContext);
	}

	effect->Deactivate();

	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_API->SetRasterizer(eRasterizer::CULL_BACK);
}

void Renderer::RenderParticles()
{
	m_API->SetBlendState(eBlendStates::ALPHA_BLEND);
	m_API->SetRasterizer(eRasterizer::CULL_NONE);

	if ( m_ProcessDirectionalShadows )
		m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	else
		m_API->SetDepthStencilState(eDepthStencilState::READ_NO_WRITE_PARTICLE, 0);

	const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::e3D);
	for ( const RenderCommand& command : commands )
	{
		switch ( command.myType )
		{
			case eType::PARTICLE:
			{
				if ( !m_ProcessDirectionalShadows )
				{
					m_ParticleEmitter->SetPosition(CU::Vector3f(256.f, 0.f, 256.f));
					m_ParticleEmitter->Update(m_Engine->GetDeltaTime());
				}
				else
				{
					m_API->GetContext()->PSSetShaderResources(1, 1, m_Engine->GetTexture("Data/Textures/hp.dds")->GetShaderViewRef());
				}
			//	m_ParticleEmitter->Render(m_ProcessDirectionalShadows ? m_DirectionalFrame : myPrevFrame, m_Camera->GetPerspective());


			} break;
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
	for ( const RenderCommand& command : commands )
	{
		switch ( command.myType )
		{
			case eType::LINE_Z_ENABLE:
			{
				m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
				my3DLine->Update(command.firstPoint, command.secondPoint);
				my3DLine->Render(m_Camera->GetOrientation(), m_Camera->GetPerspective());
			}break;
			case eType::LINE_Z_DISABLE:
			{
				m_API->SetDepthStencilState(eDepthStencilState::Z_DISABLED, 1);
				my3DLine->Update(command.firstPoint, command.secondPoint);
				my3DLine->Render(m_Camera->GetOrientation(), m_Camera->GetPerspective());
			}break;
		}
	}

	m_API->SetBlendState(eBlendStates::NO_BLEND);
	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_API->SetRasterizer(eRasterizer::CULL_BACK);
}
//
//void Renderer::ProcessShadows()
//{
//	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
//	m_ProcessShadows = true;
//
//	Camera* camera = m_Camera;
//	m_Camera = m_Shadowlight->GetCamera();
//	m_Shadowlight->SetViewport();
//
//	m_Shadowlight->ClearTexture();
//	m_Shadowlight->SetTargets();
//	//m_Shadowlight->ToggleShader(m_ProcessShadows);
//	Render3DCommands();
//	//RenderParticles();
//
//	m_Engine->ResetRenderTargetAndDepth();
//	m_API->ResetViewport();
//
//	myPrevShadowFrame = m_Camera->GetOrientation();
//	m_Camera = camera;
//
//	m_ProcessShadows = false;
//	//m_Shadowlight->ToggleShader(m_ProcessShadows);
//}
//
//void Renderer::ProcessShadows(Camera* camera)
//{
//
//	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
//	IDevContext* ctx = m_API->GetContext();
//	Camera* old_camera = m_Camera;
//	m_Camera = camera;
//	m_Shadowlight->SetViewport();
//
//	float clear[4] = { 0.f, 0.f, 0.f, 0.f };
//	ctx->ClearRenderTargetView(m_ShadowDepth->GetRenderTargetView(), clear);
//	ctx->ClearDepthStencilView(m_ShadowDepthStencil->GetDepthView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//	ctx->OMSetRenderTargets(1, m_ShadowDepth->GetRenderTargetRef(), m_ShadowDepthStencil->GetDepthView());
//
//	m_API->SetVertexShader(m_ShadowEffect->GetVertexShader()->m_Shader);
//	m_API->SetPixelShader(m_ShadowEffect->GetPixelShader()->m_Shader);
//
//	m_ShadowEffect->Activate();
//
//	m_ProcessDirectionalShadows = true;
//	Render3DCommands();
//	//RenderParticles();
//	m_ProcessDirectionalShadows = false;
//
//	m_Engine->ResetRenderTargetAndDepth();
//	m_API->ResetViewport();
//
//	m_DirectionalFrame = m_Camera->GetOrientation();
//	m_Camera = old_camera;
//
//	m_ShadowEffect->Deactivate();
//
//	myDeferredRenderer->SetBuffers();
//}

PostProcessManager& Renderer::GetPostprocessManager()
{
	return m_PostProcessManager;
}
