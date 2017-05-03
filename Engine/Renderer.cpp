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
	
// 	m_Shadowlight = new ShadowSpotlight;
// 	m_Shadowlight->Initiate(
// 		CU::Vector3f(256.f, 128.f, 256.f)
// 		, CU::Vector3f(0.f, 0.f, 1.f)
// 		, 2048.f);

	/* Directional Shadows */
	m_DirectionalCamera = new Camera;
	m_DirectionalCamera->CreateOrthographicProjection(200.f, 200.f, 1.f, 1024.f);

	m_DirectionalCamera->SetPosition({ 1024.f, 512.f, 512.f });
	m_DirectionalCamera->RotateAroundX(CL::DegreeToRad(90.f) * 1.f);

	m_ShadowDepth = new Texture;
	m_ShadowDepth->InitiateAsRenderTarget(2048.f, 2048.f, "DirectionalLight : Depth Render Target");

	m_ShadowDepthStencil = new Texture;
	m_ShadowDepthStencil->InitiateAsDepthStencil(2048.f, 2048.f, "DirectionalLight : Depthstencil View");
	/* End of Directional Shadows */

	myDeferredRenderer = new DeferredRenderer; // Where should this live?
	if (!myDeferredRenderer->Initiate(m_ShadowDepthStencil))
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

	//bool success = m_LightPass.Initiate(myDeferredRenderer->GetGBuffer(), m_Shadowlight->GetDepthStencil());
	bool success = m_LightPass.Initiate(myDeferredRenderer->GetGBuffer(), mySpotlight->GetShadowSpotlight()->GetDepthStencil());
	DL_ASSERT_EXP(success, "failed to initiate lightpass!");

	m_ParticleEmitter = new CEmitterInstance;
	m_ParticleEmitter->Initiate(mySynchronizer, myDepthTexture);

	m_Atmosphere.Initiate(724.f, 728.f, { 512.f, 0.f, 512.f });

	m_ShadowEffect = m_Engine->GetEffect("Shaders/T_Render_Depth.json");

	m_PostProcessManager.Initiate();
	m_PostProcessManager.SetPassesToProcess(PostProcessManager::HDR);



	m_RenderContext.m_API = Engine::GetAPI();
	m_RenderContext.m_Device = Engine::GetAPI()->GetDevice();
	m_RenderContext.m_Context = Engine::GetAPI()->GetContext();
	m_RenderContext.m_Engine = Engine::GetInstance();


	m_ShadowPass.RegisterFunction([&] { Render3DCommands(); });
	m_ShadowPass.RegisterFunction([&] { RenderNonDeferred3DCommands(); });
	m_ShadowPass.RegisterFunction([&] { RenderParticles(); });

	return true;
}

bool Renderer::CleanUp()
{
	SAFE_DELETE(m_DirectionalCamera);

	m_LightPass.CleanUp();
	m_PostProcessManager.CleanUp();

	m_ShadowDepthStencil->CleanUp();
	SAFE_DELETE(m_ShadowDepthStencil);

	m_ShadowDepth->CleanUp();
	SAFE_DELETE(m_ShadowDepth);

	SAFE_DELETE(my3DLine);
	SAFE_DELETE(mySprite);
	SAFE_DELETE(myClearColor);

	myDepthTexture->CleanUp();
	SAFE_DELETE(myDepthTexture);

	myDeferredRenderer->CleanUp();
	SAFE_DELETE(myDeferredRenderer);

	SAFE_DELETE(myText);

	SAFE_DELETE(myPointLight);
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
	m_API->SetDepthStencilState(eDepthStencilState::MASK_TEST, 0);

	myDeferredRenderer->SetTargets();

	Render3DCommands();

	Texture::CopyData(myDepthTexture->GetDepthTexture(), myDeferredRenderer->GetDepthStencil()->GetDepthTexture());
	ProcessShadows();
	ProcessShadows(m_DirectionalCamera);
	myDeferredRenderer->DeferredRender(myPrevFrame, m_Camera->GetPerspective(), CU::Math::Inverse(m_DirectionalCamera->GetOrientation()) * m_DirectionalCamera->GetPerspective(), m_Direction);

	/* This has to be moved or removed */
	InputHandle* input_handle = Engine::GetInstance()->GetInputHandle();
	if (input_handle->GetInputWrapper()->IsDown(KButton::Y))
	{
		CU::Vector3f target_position(512, 512.f, 512.f);
		m_DirectionalCamera->RotateAroundPoint(target_position);

		m_Direction = target_position - m_DirectionalCamera->GetPosition();
		CU::Math::Normalize(m_Direction);
	}

	RenderPointlight();
	RenderSpotlight();

	m_Engine->ResetRenderTargetAndDepth();

	myDeferredRenderer->SetBuffers(); //This is just the quad
	m_PostProcessManager.Process(myDeferredRenderer->GetFinalTexture());

	if (m_PostProcessManager.GetFlags() == 0)
		myDeferredRenderer->Finalize(0);

	
	m_Atmosphere.SetLightData(m_Direction, m_DirectionalCamera->GetPosition());
	m_Atmosphere.Render(myPrevFrame, myDepthTexture, m_RenderContext);
	
	m_API->GetContext()->OMSetRenderTargets(1, m_API->GetBackbufferRef(), myDeferredRenderer->GetDepthStencil()->GetDepthView());
	RenderParticles();

	m_Engine->ResetRenderTargetAndDepth();
	RenderNonDeferred3DCommands();
	RenderLines();
	Render2DCommands();

	//m_ShadowPass.ProcessShadows(m_DirectionalCamera, m_RenderContext);

	ImGui::Render();

	m_Engine->Present();

#ifdef _DEBUG
	SLinePoint points[2];
	points[0].position = CU::Vector4f(m_DirectionalCamera->GetPosition());
	points[0].color = CU::Vector4f(255.f, 0.f, 0.f, 255.f);
	points[1].position = CU::Vector4f(512.f, 0.f, 512.f, 1.f);
	points[1].color = CU::Vector4f(255.f, 0.f, 0.f, 255.f);

	mySynchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, points[0], points[1]));
#endif

	mySynchronizer->WaitForLogic();
	mySynchronizer->SwapBuffer();
	mySynchronizer->RenderIsDone();

	myPrevFrame = m_Camera->GetOrientation();

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

	for (const RenderCommand& command : commands)
	{
		switch (command.myType)
		{
			case eType::MODEL_NO_DEFERRED:
			{
				if (command.m_KeyOrText.empty())
				{
					TRACE_LOG("Key was empty");
					continue;
				}


				m_API->SetRasterizer(command.m_Wireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_BACK);
				m_API->SetBlendState(eBlendStates::BLEND_FALSE);

				Model* model = m_Engine->GetModel(command.m_KeyOrText);
				model->SetOrientation(command.m_Orientation);
				model->Render(myPrevFrame, m_Camera->GetPerspective(), m_RenderContext);
			}break;
		}
	}
	//EASY_END_BLOCK;
	//profiler::dumpBlocksToFile("test_profile.prof");
}

void Renderer::Render3DCommands()
{
	const CU::GrowingArray<RenderCommand>& commands = mySynchronizer->GetRenderCommands(eCommandBuffer::e3D);

	//m_API->SetRasterizer(m_RenderWireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_BACK);
	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_API->SetBlendState(eBlendStates::BLEND_FALSE);
	//m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	for (CTerrain* terrain : myTerrainArray)
	{

		if (!terrain->HasLoaded())
			continue;

		if (m_ProcessDirectionalShadows || m_ProcessShadows)
		{
			terrain->ShadowRender(m_ProcessShadows ? myPrevShadowFrame : m_DirectionalFrame, m_Camera->GetPerspective(), m_RenderContext);
		}
		else
		{
			terrain->Render(myPrevFrame, m_Camera->GetPerspective(), m_RenderContext);

		}
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


				m_API->SetBlendState(eBlendStates::BLEND_FALSE);

				Model* model = m_Engine->GetModel(command.m_KeyOrText);
				model->SetOrientation(command.m_Orientation);


				if (m_ProcessDirectionalShadows)
				{

					m_API->SetRasterizer(eRasterizer::CULL_NONE);
					model->ShadowRender(m_DirectionalFrame, m_Camera->GetPerspective(), m_RenderContext);
				}
				else if (m_ProcessShadows)
				{

					m_API->SetRasterizer(eRasterizer::CULL_NONE);
					model->ShadowRender(myPrevShadowFrame, m_Camera->GetPerspective(), m_RenderContext);
				}
				else
				{
					m_API->SetRasterizer(command.m_Wireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_BACK);
					model->Render(myPrevFrame, m_Camera->GetPerspective(), m_RenderContext);
				}
			}break;
		}
	}
}

void Renderer::Render2DCommands()
{
	const CU::GrowingArray<RenderCommand>& commands2D = mySynchronizer->GetRenderCommands(eCommandBuffer::e2D);
	m_API->SetRasterizer(eRasterizer::CULL_NONE);
	m_API->SetDepthStencilState(eDepthStencilState::Z_DISABLED, 0);
	m_API->SetBlendState(eBlendStates::NO_BLEND);
	for (const RenderCommand& command : commands2D)
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
	Effect* effect = m_LightPass.GetSpotlightEffect();
	effect->Activate();

	SpotlightData data;
	for (const RenderCommand& command : commands)
	{
		DL_ASSERT_EXP(command.myType == eType::SPOTLIGHT, "Wrong command type in spotlight buffer.");

		data.myAngle = command.myAngle;
		data.myRange = command.myRange;
		data.myLightColor = command.myColor;
		data.myLightPosition = command.myPosition;
		data.myOrientation = command.m_Orientation;

		mySpotlight->SetData(data);

		ShadowSpotlight* shadow = mySpotlight->GetShadowSpotlight();
		m_ShadowPass.ProcessShadows(shadow->GetCamera(), m_RenderContext);
		m_LightPass.RenderSpotlight(mySpotlight, m_Camera, myPrevFrame, shadow->GetMVP(), m_RenderContext);
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

	for (const RenderCommand& command : commands)
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
	for (const RenderCommand& command : commands)
	{
		switch (command.myType)
		{
			case eType::PARTICLE:
			{
				if (!m_ProcessDirectionalShadows )
				{
					m_ParticleEmitter->SetPosition(CU::Vector3f(256.f, 0.f, 256.f));
					m_ParticleEmitter->Update(m_Engine->GetDeltaTime());
				}
				else
				{
					m_API->GetContext()->PSSetShaderResources(1, 1, m_Engine->GetTexture("Data/Textures/hp.dds")->GetShaderViewRef());
				}
				m_ParticleEmitter->Render(m_ProcessDirectionalShadows ? m_DirectionalFrame : myPrevFrame, m_Camera->GetPerspective());


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
	for (const RenderCommand& command : commands)
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
	//BROFILER_FRAME("Renderer::ProcessShadows");

	m_API->SetDepthStencilState(eDepthStencilState::MASK_TEST, 0);
	m_ProcessShadows = true;

	Camera* camera = m_Camera;
	m_Camera = m_Shadowlight->GetCamera();
	m_Shadowlight->SetViewport();

	m_Shadowlight->ClearTexture();
	m_Shadowlight->SetTargets();
	//m_Shadowlight->ToggleShader(m_ProcessShadows);
	Render3DCommands();
	//RenderParticles();

	m_Engine->ResetRenderTargetAndDepth();
	m_API->ResetViewport();

	myPrevShadowFrame = m_Camera->GetOrientation();
	m_Camera = camera;

	m_ProcessShadows = false;
	//m_Shadowlight->ToggleShader(m_ProcessShadows);
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
	RenderParticles();
	m_ProcessDirectionalShadows = false;

	m_Engine->ResetRenderTargetAndDepth();
	m_API->ResetViewport();

	m_DirectionalFrame = m_Camera->GetOrientation();
	m_Camera = old_camera;

	m_ShadowEffect->Deactivate();

	myDeferredRenderer->SetBuffers();
}

PostProcessManager& Renderer::GetPostprocessManager()
{
	return m_PostProcessManager;
}
