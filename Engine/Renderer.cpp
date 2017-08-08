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
#include <Engine/WaterPlane.h>

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
	window_size.m_Height = m_API->GetInfo().m_WindowHeight;
	window_size.m_Width = m_API->GetInfo().m_WindowWidth;

	myText = new CText("Data/Font/OpenSans-Bold.ttf", 8, 1);

	myPointLight = new PointLight; //Where should this live?

	m_DeferredRenderer = new DeferredRenderer; // Where should this live?
	if (!m_DeferredRenderer->Initiate(0))
		return false;

	myDepthTexture = new Texture; //Where should this live?
	DL_ASSERT_EXP(myDepthTexture, "DepthTexture in Renderer was null?");

	myDepthTexture->Initiate(window_size.m_Width, window_size.m_Height
		, DEFAULT_USAGE | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL
		, DXGI_FORMAT_R32_TYPELESS
		, DXGI_FORMAT_R32_FLOAT
		, DXGI_FORMAT_D32_FLOAT
		, "Renderer : Depth");


	m_SpriteHeight = 270.f;
	m_SpriteWidth = 480.f;

	mySprite = new Sprite;
	mySprite->Initiate("Data/Textures/colors.dds", CU::Vector2f(m_SpriteWidth, m_SpriteHeight), CU::Vector2f(0.f, 0.f));

	myClearColor = new Sprite;
	myClearColor->Initiate("Data/Textures/blank.dds", CU::Vector2f(128.f, 128.f), CU::Vector2f(0.f, 0.f));

	m_Line = new CLine3D; //Where should this live?
	m_Line->Initiate();

	bool success = m_LightPass.Initiate(m_DeferredRenderer->GetGBuffer(), 0);
	DL_ASSERT_EXP(success, "failed to initiate lightpass!");

	m_ParticleEmitter = new CEmitterInstance;
	m_ParticleEmitter->Initiate(mySynchronizer, myDepthTexture);

	m_Atmosphere.Initiate(1024.f, 1024.f, { 512.f, 0.f, 512.f });


	m_PostProcessManager.Initiate();
	m_PostProcessManager.SetPassesToProcess(PostProcessManager::HDR);

	m_RenderContext.m_API = Engine::GetAPI();
	m_RenderContext.m_Device = Engine::GetAPI()->GetDevice();
	m_RenderContext.m_Context = Engine::GetAPI()->GetContext();
	m_RenderContext.m_Engine = Engine::GetInstance();

	m_ShadowPass.Initiate(this);

	m_DirectionalShadow.Initiate(2048.f);

#if !defined(_PROFILE) && !defined(_FINAL)
	const GBuffer& gbuffer = m_DeferredRenderer->GetGBuffer();
	m_Engine->AddTexture(gbuffer.GetDiffuse(), "Scene Diffuse");
	m_Engine->AddTexture(gbuffer.GetNormal(), "Scene Normal");
	m_Engine->AddTexture(gbuffer.GetDepth(), "Scene Depth");
	m_Engine->AddTexture(gbuffer.GetEmissive(), "Scene Emissive");
#endif

	m_WaterPlane = new WaterPlane;
	m_WaterPlane->Initiate({ -512, 0, -512 });

	m_WaterCamera = new Camera;
	m_WaterCamera->CreatePerspectiveProjection(window_size.m_Width, window_size.m_Height, 0.01f, 100.f, 90.f);


	m_ParticleBuffer = new Texture;
	m_ParticleBuffer->Initiate(window_size.m_Width, window_size.m_Height, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R16G16B16A16_UNORM, "Particle Render Target");


	m_ParticleDiff = new Texture;
	m_ParticleDiff->InitiateAsRenderTarget(window_size.m_Width, window_size.m_Height, "Particle Depth RTV");
	//, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R16G16B16A16_UNORM, "Particle Render Target");

	m_ParticleDepth = new Texture;
	//m_ParticleDepth->Initiate(window_size.m_Width, window_size.m_Height, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT, "Particle Depth Render Target");
	m_ParticleDepth->Initiate(window_size.m_Width, window_size.m_Height
		, DEFAULT_USAGE | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL
		, DXGI_FORMAT_R32_TYPELESS
		, DXGI_FORMAT_R32_FLOAT
		, DXGI_FORMAT_D32_FLOAT
		, "Particle : DepthStencil ");


	Engine::GetInstance()->GetEffect("Shaders/particle.json")->AddShaderResource(m_ParticleBuffer, Effect::NORMAL);
	Engine::GetInstance()->GetEffect("Shaders/particle.json")->AddShaderResource(myDepthTexture, Effect::DEPTH);
	Engine::GetInstance()->GetEffect("Shaders/deferred_pointlight.json")->AddShaderResource(m_ParticleBuffer, Effect::PARTICLES);
	Engine::GetInstance()->GetEffect("Shaders/deferred_spotlight.json")->AddShaderResource(m_ParticleBuffer, Effect::PARTICLES);
	m_Engine->AddTexture(m_ParticleBuffer, "Particle Normal");
	m_Engine->AddTexture(m_ParticleDepth, "Particle Depth pass Stencil");
	m_Engine->AddTexture(m_ParticleDiff, "Particle Depth pass RTV");

	m_Quad = new Quad;
	m_Quad->Initiate();


	m_cbParticleBuf = m_RenderContext.m_API->CreateConstantBuffer(sizeof(m_cbParticles));


	m_CalcSSNormal.m_TexelSize.x = 1.f / window_size.m_Width;
	m_CalcSSNormal.m_TexelSize.y = 1.f / window_size.m_Height;
	m_cbCalcSSNormal = m_RenderContext.m_API->CreateConstantBuffer(sizeof(m_CalcSSNormal));


	Effect* fx = m_Engine->GetEffect("Shaders/particle_normal.json");
	fx->AddShaderResource(m_ParticleDiff, Effect::DEPTH);


	//m_Engine->GetEffect("Shaders/T_particle_offscreen.json")->AddShaderResource(myDepthTexture, Effect::DEPTH);

	m_Direction = CU::Vector3f(0.42f, 0.73f, 0.24f);


	//10,5,10

	Camera* c = m_DirectionalShadow.GetCamera();
	//c->SetPosition(CU::Vector3f(0, 0, -5));
	//c->RotateAroundX(CL::DegreeToRad(90.f) * -1.0f);
	//c->RotateAroundY(CL::DegreeToRad(90.f) * 0.42f);
	//c->RotateAroundZ(CL::DegreeToRad(90.f) * 0.73f);


	return true;
}
//_________________________________

bool Renderer::CleanUp()
{
	m_Quad->CleanUp();
	delete m_Quad;
	SAFE_RELEASE(m_cbParticleBuf);
	m_ParticleBuffer->CleanUp();
	SAFE_DELETE(m_ParticleBuffer);
	m_LightPass.CleanUp();
	m_ShadowPass.CleanUp();
	m_DirectionalShadow.CleanUp();
	m_PostProcessManager.CleanUp();
	m_WaterPlane->CleanUp();
	SAFE_DELETE(m_WaterCamera);
	SAFE_DELETE(m_WaterPlane);
	SAFE_DELETE(m_Line);
	SAFE_DELETE(mySprite);
	SAFE_DELETE(myClearColor);

	myDepthTexture->CleanUp();
	SAFE_DELETE(myDepthTexture);

	m_DeferredRenderer->CleanUp();
	SAFE_DELETE(m_DeferredRenderer);
	SAFE_DELETE(m_WaterPlane);

	SAFE_DELETE(myText);

	SAFE_DELETE(myPointLight);

	m_ParticleEmitter->CleanUp();
	SAFE_DELETE(m_ParticleEmitter);


	for (SpotLight* s : m_Spotlights)
	{
		s->CleanUp();
		SAFE_DELETE(s);
	}

	m_Atmosphere.CleanUp();

	return true;
}

void Renderer::Render()
{
#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Magenta);
#endif
	m_Engine->Clear();


	//ProcessWater();


	m_DeferredRenderer->SetGBufferAsTarget(m_RenderContext);

	RenderTerrain(false);
	//m_WaterPlane->Render(m_Camera->GetOrientation(), m_Camera->GetPerspective(), m_RenderContext);

	if (m_Engine->GetRenderInstanced())
		Render3DCommandsInstanced();
	else
		Render3DCommands();

	m_Atmosphere.Render(m_Camera->GetOrientation(), m_DeferredRenderer->GetDepthStencil(), m_RenderContext);

	Texture::CopyData(myDepthTexture->GetDepthTexture(), m_DeferredRenderer->GetDepthStencil()->GetDepthTexture());

	/*const float clear[4] = { 0.f,0.f,0.f,0.f };

	m_RenderContext.m_Context->ClearRenderTargetView(m_ParticleDiff->GetRenderTargetView(), clear);
	m_RenderContext.m_Context->ClearDepthStencilView(m_ParticleDepth->GetDepthView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0.f);
	m_RenderContext.m_Context->OMSetRenderTargets(1, m_ParticleDiff->GetRenderTargetRef(), m_ParticleDepth->GetDepthView());*/

	//RenderParticles(m_Engine->GetEffect("Shaders/particle_offscreen.json")); //Should be a depth shader

	/*m_Quad->SetBuffers();
	Effect* fx = m_Engine->GetEffect("Shaders/particle_normal.json");

	m_CalcSSNormal.m_InvProjection = CU::Math::InverseReal(m_Camera->GetPerspective());
	m_CalcSSNormal.m_Projection = m_Camera->GetPerspective();

	m_RenderContext.m_API->UpdateConstantBuffer(m_cbCalcSSNormal, &m_CalcSSNormal);
	m_RenderContext.m_Context->PSSetConstantBuffers(0, 1, &m_cbCalcSSNormal);
	m_RenderContext.m_Context->ClearRenderTargetView(m_ParticleBuffer->GetRenderTargetView(), clear);
	ID3D11RenderTargetView* view[] = {
		m_ParticleBuffer->GetRenderTargetView()
	};
	m_RenderContext.m_Context->OMSetRenderTargets(ARRAYSIZE(view), &view[0], myDepthTexture->GetDepthView());
	fx->Use();
	m_RenderContext.m_API->SetBlendState(eBlendStates::NO_BLEND);
	m_Quad->Render();
	fx->Clear();*/


	//mySynchronizer->AddRenderCommand(SpriteCommand(m_DirectionalShadow.GetDepthTexture()->GetShaderView(), CU::Vector2f(1920 / 2, 1080 / 2)));
	//mySynchronizer->AddRenderCommand(SpriteCommand(myDepthTexture->GetShaderView(), CU::Vector2f(1920 / 2, 1080 / 4)));

	/*
		Render Particles to depth
		Calculate normals based on neighboring texel of the depth map
		Render to screen
	*/
	/**

	m_RenderContext.m_Context->ClearRenderTargetView(m_ParticleBuffer->GetRenderTargetView(), clear);
	m_RenderContext.m_Context->ClearRenderTargetView(m_ParticleDiff->GetRenderTargetView(), clear);

	ID3D11RenderTargetView* view[] = {
		m_ParticleDiff->GetRenderTargetView(),
		m_ParticleBuffer->GetRenderTargetView()
	};
	m_RenderContext.m_Context->OMSetRenderTargets(ARRAYSIZE(view), &view[0], myDepthTexture->GetDepthView());
	RenderParticles(m_Engine->GetEffect("Shaders/T_particle_offscreen.json"));
	*/

	m_ShadowPass.ProcessShadows(&m_DirectionalShadow, m_RenderContext);

	CU::Matrix44f shadow_mvp = m_DirectionalShadow.GetMVP();


	m_DeferredRenderer->DeferredRender(
		m_Camera->GetOrientation(),
		m_Camera->GetPerspective(),
		shadow_mvp,
		m_Direction,
		m_RenderContext);

	RenderPointlight();
	RenderSpotlight();

	m_Engine->ResetRenderTargetAndDepth();
	m_DeferredRenderer->SetBuffers(m_RenderContext); //This is just the quad

	m_PostProcessManager.Process(m_DeferredRenderer->GetFinalTexture());

	if (m_PostProcessManager.GetFlags() == 0)
		m_DeferredRenderer->Finalize(m_RenderContext);

	m_API->GetContext()->OMSetRenderTargets(1, m_API->GetBackbufferRef(), m_DeferredRenderer->GetDepthStencil()->GetDepthView());

	m_Engine->ResetRenderTargetAndDepth();

	RenderNonDeferred3DCommands();



	//m_Quad->SetBuffers();


	//ID3D11ShaderResourceView* srv[] = {
	//	m_ParticleDiff->GetShaderView(),
	//	m_ParticleBuffer->GetShaderView(),
	//};

	//m_RenderContext.m_API->SetVertexShader(m_Quad->GetShader()->GetVertexShader());
	//m_RenderContext.m_API->SetPixelShader(Engine::GetInstance()->GetEffect("Shaders/T_Particle.json")->GetPixelShader());
	//m_RenderContext.m_Context->PSSetShaderResources(0, ARRAYSIZE(srv), &srv[0]);

	//m_cbParticles.m_ViewDir = m_Camera->GetAt();
	//m_cbParticles.invProjection = CU::Math::InverseReal(m_Camera->GetPerspective());
	//m_cbParticles.view = m_Camera->GetOrientation();

	//m_RenderContext.m_API->UpdateConstantBuffer(m_cbParticleBuf, &m_cbParticles);

	//m_RenderContext.m_Context->PSSetConstantBuffers(0, 1, &m_cbParticleBuf);

	//m_Quad->Render();


	//RenderParticles(m_Engine->GetEffect("Shaders/T_Particle.json"));
	RenderLines();
	Render2DCommands();

#if !defined(_PROFILE) && !defined(_FINAL)
	ImGui::Render();
#endif
	m_Engine->Present();

#ifdef _PROFILE
	EASY_BLOCK("Waiting for Logic!");
#endif
	mySynchronizer->WaitForLogic();
#ifdef _PROFILE
	EASY_END_BLOCK;
#endif
	Engine::GetInstance()->GetMemorySegmentHandle().Clear((s32)mySynchronizer->GetCurrentBufferIndex());
	mySynchronizer->SwapBuffer();
	mySynchronizer->RenderIsDone();

}

void Renderer::ProcessWater()
{
	memcpy(m_WaterCamera, m_Camera, sizeof(Camera)); //This seem extremely unsafe!
	Camera* old_camera = m_Camera;
	m_Camera = m_WaterCamera;


	m_WaterPlane->SetupRefractionRender(m_RenderContext);
	m_WaterPlane->SetClipPlane({ 0.f, -1.f, 0.f, 2.f }, m_RenderContext);
	RenderTerrain(true);

	Render3DCommandsInstanced();

	CU::Vector3f position0 = old_camera->GetPosition();
	m_Camera->SetPosition(position0);

	float distance = 2 * (position0.y - m_WaterPlane->GetPosition().y);
	position0.y -= distance;
	m_Camera->SetPosition(position0);
	m_Camera->InvertAll();
	m_WaterPlane->SetupReflectionRender(m_RenderContext);
	m_WaterPlane->SetClipPlane({ 0.f, 1.f, 0.f, 2.f }, m_RenderContext);
	RenderTerrain(true);

	Render3DCommandsInstanced();
	m_Atmosphere.Render(m_Camera->GetOrientation(), myDepthTexture, m_RenderContext);

	position0.y += distance;
	m_Camera->SetPosition(position0);


	m_Camera = old_camera;
}

//_________________________________

void Renderer::AddTerrain(Terrain* someTerrain)
{
	myTerrainArray.Add(someTerrain);
}
//_________________________________

void Renderer::RenderNonDeferred3DCommands()
{

#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Amber);
#endif

	m_API->EnableZBuffer();
	const auto commands = mySynchronizer->GetRenderCommands(eBufferType::NO_DEFERRED_BUFFER);
	for (s32 i = 0; i < commands.Size(); i++)
	{
		auto command = reinterpret_cast<ModelCommandNonDeferred*>(commands[i]);
		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::MODEL, "Incorrect command type! Expected MODEL");

		m_API->SetBlendState(eBlendStates::BLEND_FALSE);
		Model* model = m_Engine->GetModel(command->m_Key);
		model->SetOrientation(command->m_Orientation);
		m_API->SetRasterizer(command->m_Wireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_BACK);
		model->Render(m_Camera->GetOrientation(), m_Camera->GetPerspective(), m_RenderContext);

	}
}
//_________________________________

void Renderer::Render3DCommands()
{
#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Green);
#endif
	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_API->SetBlendState(eBlendStates::BLEND_FALSE);

#ifdef _PROFILE
	EASY_BLOCK("RenderModels", profiler::colors::Amber);
#endif

	const CU::Matrix44f& orientation = m_Camera->GetOrientation();
	const CU::Matrix44f& perspective = m_Camera->GetPerspective();

	const u16 current_buffer = Engine::GetInstance()->GetSynchronizer()->GetCurrentBufferIndex();
	for (s32 j = 0; j < 8; j++)
	{
		const auto& commands = Engine::GetInstance()->GetMemorySegmentHandle().GetCommandAllocator(current_buffer, j);

		for (s32 i = 0; i < commands.Size(); i++)
		{
			auto command = reinterpret_cast<ModelCommand*>(commands[i]);
			DL_ASSERT_EXP(command->m_CommandType == RenderCommand::MODEL, "Incorrect command type! Expected MODEL");

			m_API->SetBlendState(eBlendStates::BLEND_FALSE);
			Model* model = m_Engine->GetModel(command->m_Key);
			model->SetOrientation(command->m_Orientation);
			m_API->SetRasterizer(command->m_Wireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_BACK);
			model->Render(orientation, perspective, m_RenderContext);
		}
	}

#ifdef _PROFILE
	EASY_END_BLOCK;
#endif

}
//_________________________________

void Renderer::Render3DCommandsInstanced()
{
#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Green);
#endif
	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_API->SetBlendState(eBlendStates::BLEND_FALSE);

#ifdef _PROFILE
	EASY_BLOCK("RenderModels", profiler::colors::Amber);
#endif


	const u16 current_buffer = Engine::GetInstance()->GetSynchronizer()->GetCurrentBufferIndex();
	for (s32 j = 0; j < 8; j++)
	{
		const auto& commands = Engine::GetInstance()->GetMemorySegmentHandle().GetCommandAllocator(current_buffer, j);
		for (s32 i = 0; i < commands.Size(); i++)
		{
			ProcessCommand(commands, i);
		}
	}

	const CU::Matrix44f& orientation = m_Camera->GetOrientation();
	const CU::Matrix44f& perspective = m_Camera->GetPerspective();

	for (auto it = m_ModelsToRender.begin(); it != m_ModelsToRender.end(); it++)
	{
		m_API->SetBlendState(eBlendStates::BLEND_FALSE);
		m_API->SetRasterizer(eRasterizer::CULL_NONE); //set per model instance? Array with bools / byte to see if it is wireframe or not?
		it->second->RenderInstanced(orientation, perspective, m_RenderContext);
	}
}

void Renderer::ProcessCommand(const memory::CommandAllocator& commands, s32 i)
{
	auto command = reinterpret_cast<ModelCommand*>(commands[i]);
	const bool result = (command->m_CommandType == RenderCommand::MODEL);
	DL_ASSERT_EXP(result == true, "Incorrect command type! Expected MODEL");
	Model* model = m_Engine->GetModel(command->m_Key);
	model->AddOrientation(command->m_Orientation);
	if (m_ModelsToRender.find(command->m_Key) == m_ModelsToRender.end())
		m_ModelsToRender.emplace(command->m_Key, model);
}

//_________________________________

void Renderer::RenderTerrain(bool override_effect)
{
	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_API->SetBlendState(eBlendStates::BLEND_FALSE);
	m_API->SetRasterizer(eRasterizer::CULL_BACK);
#ifdef _PROFILE
	EASY_FUNCTION();
#endif
	for (Terrain* terrain : myTerrainArray)
	{
		if (!terrain->HasLoaded())
			continue;
		if (!override_effect)
			terrain->Render(m_Camera->GetOrientation(), m_Camera->GetPerspective(), m_RenderContext);
		else
			terrain->Render(m_Camera->GetOrientation(), m_Camera->GetPerspective(), m_RenderContext, true);

	}
}

void Renderer::Render3DShadows(const CU::Matrix44f& orientation, Camera* camera)
{
	const auto commands = mySynchronizer->GetRenderCommands(eBufferType::MODEL_BUFFER);
	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_API->SetBlendState(eBlendStates::BLEND_FALSE);
	m_API->SetRasterizer(eRasterizer::CULL_NONE);

	/*for (s32 i = 0; i < commands.Size(); i++)
	{
		auto command = reinterpret_cast<ModelCommand*>(commands[i]);


		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::MODEL, "Incorrect command type! Expected MODEL");

		Model* model = m_Engine->GetModel(command->m_Key);
		if (!model)
			continue;

		model->SetOrientation(command->m_Orientation);
		model->ShadowRender(orientation, camera->GetPerspective(), m_RenderContext);
	}*/

	const u16 current_buffer = Engine::GetInstance()->GetSynchronizer()->GetCurrentBufferIndex();
	for (s32 j = 0; j < 8; j++)
	{
		const auto& commands = Engine::GetInstance()->GetMemorySegmentHandle().GetCommandAllocator(current_buffer, j);
		for (s32 i = 0; i < commands.Size(); i++)
		{
			ProcessCommand(commands, i);
		}
	}

	const CU::Matrix44f& perspective = camera->GetPerspective();
	for (auto it = m_ModelsToRender.begin(); it != m_ModelsToRender.end(); it++)
	{
		it->second->ShadowRenderInstanced(orientation, perspective, m_RenderContext);
	}

}
//_________________________________

int Renderer::RegisterLight()
{
	SpotLight* s = new SpotLight;
	s->Initiate();
	m_Spotlights.Add(s);
	return (m_Spotlights.Size() - 1);
}
//_________________________________

void Renderer::Render2DCommands()
{

	m_API->SetRasterizer(eRasterizer::CULL_NONE);
	m_API->SetDepthStencilState(eDepthStencilState::Z_DISABLED, 0);
	m_API->SetBlendState(eBlendStates::NO_BLEND);

	//_________________________
	// RenderSpriteCommands function?
	const auto commands0 = mySynchronizer->GetRenderCommands(eBufferType::SPRITE_BUFFER);
	for (s32 i = 0; i < commands0.Size(); i++)
	{
		auto command = reinterpret_cast<SpriteCommand*>(commands0[i]);
		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::SPRITE, "Expected Sprite command type");
		mySprite->SetPosition(command->m_Position);
		mySprite->SetShaderView(command->m_Resource);
		mySprite->Render(m_Camera);
	}

	//_________________________
	// RenderTextCommands function?
	const auto commands1 = mySynchronizer->GetRenderCommands(eBufferType::TEXT_BUFFER);
	for (s32 i = 0; i < commands1.Size(); i++)
	{
		auto command = reinterpret_cast<TextCommand*>(commands1[i]);
		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::TEXT, "Expected Text command type");
		myText->SetText(command->m_TextBuffer);
		myText->SetPosition(command->m_Position);
		myText->Render(m_Camera);
	}

	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_API->SetRasterizer(eRasterizer::CULL_BACK);
}
//_________________________________

void Renderer::RenderSpotlight()
{
#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Purple);
#endif
	Effect* effect = m_LightPass.GetSpotlightEffect();

	SpotlightData data;
	const auto commands = mySynchronizer->GetRenderCommands(eBufferType::SPOTLIGHT_BUFFER);
#ifdef _PROFILE
	EASY_BLOCK("Spotlight Command Loop", profiler::colors::Red);
#endif
	for (s32 i = 0; i < commands.Size(); i++)
	{
#ifdef _PROFILE
		EASY_BLOCK("Spotlight Command", profiler::colors::Red);
#endif
		auto command = reinterpret_cast<SpotlightCommand*>(commands[i]);
		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::SPOTLIGHT, "Expected Spotlight command type");

		data.myAngle = command->m_Angle;
		data.myRange = command->m_Range;
		data.myLightColor = command->m_Color;
		data.myLightPosition = command->m_Orientation.GetPosition();
		data.myOrientation = command->m_Orientation;

		SpotLight* light = m_Spotlights[command->m_LightID];
		light->SetData(data);

		CU::Matrix44f shadow_mvp;
		if (command->m_ShadowCasting)
		{
			ShadowSpotlight* shadow = light->GetShadowSpotlight();
			m_ShadowPass.ProcessShadows(shadow, m_RenderContext);
			shadow_mvp = shadow->GetMVP();
			effect->AddShaderResource(shadow->GetDepthStencil(), Effect::SHADOWMAP);
			m_DeferredRenderer->SetRenderTarget(m_RenderContext);
		}

		m_API->SetRasterizer(eRasterizer::CULL_NONE);
		m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 0);
		//m_LightState.Use(m_RenderContext);
		effect->Use();
		m_LightPass.RenderSpotlight(light, m_Camera, m_Camera->GetOrientation(), shadow_mvp, m_RenderContext);
		effect->Clear();

#ifdef _PROFILE
		EASY_END_BLOCK;
#endif
	}
#ifdef _PROFILE
	EASY_END_BLOCK;
#endif

	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_API->SetRasterizer(eRasterizer::CULL_BACK);
}
//_________________________________

void Renderer::RenderPointlight()
{
#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Purple);
#endif
	const auto commands = mySynchronizer->GetRenderCommands(eBufferType::POINTLIGHT_BUFFER);


	m_API->SetRasterizer(eRasterizer::CULL_NONE);
	m_API->SetDepthStencilState(eDepthStencilState::READ_NO_WRITE, 0);
	Effect* effect = m_LightPass.GetPointlightEffect();
	effect->Use();
#ifdef _PROFILE
	EASY_BLOCK("Pointlight Command Loop", profiler::colors::Red);
#endif
	for (s32 i = 0; i < commands.Size(); i++)
	{
		auto command = reinterpret_cast<PointlightCommand*>(commands[i]);

		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::POINTLIGHT, "Wrong command type in pointlight buffer.");
		m_API->SetBlendState(eBlendStates::LIGHT_BLEND);
		myPointLight->SetPosition(command->m_Orientation.GetPosition());
		myPointLight->SetRange(command->m_Range);
		myPointLight->SetColor(CU::Vector4f(command->m_Color.x, command->m_Color.y, command->m_Color.z, 1));
		myPointLight->Update();
		CU::Matrix44f shadow_mvp;
		m_LightPass.RenderPointlight(myPointLight, m_Camera, m_Camera->GetOrientation(), shadow_mvp, m_RenderContext);
	}
#ifdef _PROFILE
	EASY_END_BLOCK;
#endif
	effect->Clear();

	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_API->SetRasterizer(eRasterizer::CULL_BACK);


}
//_________________________________

void Renderer::RenderParticles(Effect* effect)
{

	m_API->SetBlendState(eBlendStates::PARTICLE_BLEND);
	m_API->SetRasterizer(eRasterizer::CULL_NONE);
	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 0);

	const auto commands = mySynchronizer->GetRenderCommands(eBufferType::PARTICLE_BUFFER);
	for (s32 i = 0; i < commands.Size(); i++)
	{
		auto command = reinterpret_cast<ParticleCommand*>(commands[i]);
		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::PARTICLE, "Expected particle command type");
		m_ParticleEmitter->SetPosition(command->m_Position);

		m_ParticleEmitter->Update(m_Engine->GetDeltaTime());

		//if ( !m_ProcessDirectionalShadows )
		//{
		//}
		//else
		//{
		//	m_API->GetContext()->PSSetShaderResources(1, 1, m_Engine->GetTexture("Data/Textures/hp.dds")->GetShaderViewRef());
		//}
		m_ParticleEmitter->Render(m_Camera->GetOrientation(), m_Camera->GetPerspective(), effect);
	}
	m_API->SetRasterizer(eRasterizer::CULL_BACK);

	//reset

}
//_________________________________

void Renderer::RenderLines()
{
#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Amber);
#endif

	m_API->SetBlendState(eBlendStates::NO_BLEND);
	m_API->SetRasterizer(eRasterizer::CULL_NONE);

	ID3D11RenderTargetView* backbuffer = m_API->GetBackbuffer();
	ID3D11DepthStencilView* depth = m_DeferredRenderer->GetDepthStencil()->GetDepthView();
	m_API->GetContext()->OMSetRenderTargets(1, &backbuffer, depth);

	const auto commands = mySynchronizer->GetRenderCommands(eBufferType::LINE_BUFFER);
#ifdef _PROFILE
	EASY_BLOCK("Line Command Loop", profiler::colors::Red);
#endif
	for (s32 i = 0; i < commands.Size(); i++)
	{
		auto command = reinterpret_cast<LineCommand*>(commands[i]);
		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::LINE, "Expected Line command type");
		m_API->SetDepthStencilState(command->m_ZEnabled ? eDepthStencilState::Z_ENABLED : eDepthStencilState::Z_DISABLED, 1);
		m_Line->Update(command->m_Points[0], command->m_Points[1]);
		m_Line->Render(m_Camera->GetOrientation(), m_Camera->GetPerspective());
	}
#ifdef _PROFILE
	EASY_END_BLOCK;
#endif


	m_API->SetBlendState(eBlendStates::NO_BLEND);
	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_API->SetRasterizer(eRasterizer::CULL_BACK);
}
//_________________________________

PostProcessManager& Renderer::GetPostprocessManager()
{
	return m_PostProcessManager;
}
