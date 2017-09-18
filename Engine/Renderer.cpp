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
#include <Engine/IGraphicsContext.h>

Renderer::Renderer(Synchronizer* synchronizer)
	: m_Synchronizer(synchronizer)
	, m_Camera(Engine::GetInstance()->GetCamera())
{
	auto api = Engine::GetAPI();
	m_RenderContext = graphics::RenderContext(Engine::GetInstance(),
											  api->GetDevice(),
											  api->GetContext(),
											  api);

	myText = new CText("Data/Font/OpenSans-Bold.ttf", 8, 1);

	m_DeferredRenderer = new DeferredRenderer;

	WindowSize window_size;
	window_size.m_Height = api->GetInfo().m_WindowHeight;
	window_size.m_Width = api->GetInfo().m_WindowWidth;

	m_DepthTexture = new Texture; //Where should this live?
	m_DepthTexture->InitiateAsDepthStencil(window_size.m_Width, window_size.m_Height, "Renderer : Depth");


	m_PostProcessManager.Initiate();
	m_PostProcessManager.SetPassesToProcess(PostProcessManager::HDR); //Can be read from a settings file




	m_Line = new CLine3D; //Where should this live?
	m_Line->Initiate();

	m_LightPass = graphics::LightPass(m_GBuffer);

	m_ParticleEmitter = new CEmitterInstance;
	m_ParticleEmitter->Initiate(m_Synchronizer, m_DepthTexture);

	m_Atmosphere.Initiate(1024.f, 1024.f, { 512.f, 0.f, 512.f });



	m_ShadowPass.Initiate(this);

	m_DirectionalShadow.Initiate(2048.f);

	m_Direction = CU::Vector3f(0.42f, 0.73f, 0.24f);

#if !defined(_PROFILE) && !defined(_FINAL)
	m_DebugTexture0 = new Texture;
	m_DebugTexture0->InitiateAsRenderTarget(window_size.m_Width, window_size.m_Height, "diffuse, albedo");

	m_DebugTexture1 = new Texture;
	m_DebugTexture1->InitiateAsRenderTarget(window_size.m_Width, window_size.m_Height, "normal");

	m_DebugTexture2 = new Texture;
	m_DebugTexture2->InitiateAsRenderTarget(window_size.m_Width, window_size.m_Height, "depth");

	m_DebugTexture3 = new Texture;
	m_DebugTexture3->InitiateAsRenderTarget(window_size.m_Width, window_size.m_Height, "metalness");

	m_DebugTexture4 = new Texture;
	m_DebugTexture4->InitiateAsRenderTarget(window_size.m_Width, window_size.m_Height, "roughness");

	Effect* debug_textures = m_Engine->GetEffect("Shaders/debug_textures.json");
	debug_textures->AddShaderResource(m_GBuffer.GetDiffuse(), Effect::DIFFUSE);
	debug_textures->AddShaderResource(m_GBuffer.GetNormal(), Effect::NORMAL);
	debug_textures->AddShaderResource(m_GBuffer.GetDepth(), Effect::DEPTH);
	debug_textures->AddShaderResource(m_GBuffer.GetEmissive(), Effect::EMISSIVE);

	m_DebugQuad = new Quad(Engine::GetInstance()->GetEffect("Shaders/debug_textures.json"));
#endif

	m_PBLValues = m_Engine->GetAPI()->CreateConstantBuffer(sizeof(m_values));
}

Renderer::~Renderer()
{

#if !defined(_PROFILE) && !defined(_FINAL)
	SAFE_DELETE(m_DebugQuad);
	SAFE_DELETE(m_DebugTexture0);
	SAFE_DELETE(m_DebugTexture1);
	SAFE_DELETE(m_DebugTexture2);
	SAFE_DELETE(m_DebugTexture3);
	SAFE_DELETE(m_DebugTexture4);
#endif

	m_ShadowPass.CleanUp();
	m_DirectionalShadow.CleanUp();
	m_PostProcessManager.CleanUp();

	SAFE_DELETE(m_WaterCamera);
	SAFE_DELETE(m_Line);

	SAFE_DELETE(m_DepthTexture);

	SAFE_DELETE(m_DeferredRenderer);

	SAFE_DELETE(myText);

	m_ParticleEmitter->CleanUp();
	SAFE_DELETE(m_ParticleEmitter);


	for (SpotLight* s : m_Spotlights)
	{
		SAFE_DELETE(s);
	}
}

void Renderer::Render()
{

	if (m_Synchronizer->HasQuit())
	{
		m_Synchronizer->RenderIsDone();
		return;
	}

#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Magenta);
#endif
	//m_Engine->Clear();


	//ProcessWater();

	//m_GBuffer.SetAsRenderTarget();
	//m_DeferredRenderer->SetGBufferAsTarget(m_RenderContext);

	//m_WaterPlane->Render(m_Camera->GetOrientation(), m_Camera->GetPerspective(), m_RenderContext);

	//Render3DCommandsInstanced();
	/*else
		Render3DCommands();
*/


	//memcpy(m_DepthTexture->GetDepthTexture(), m_DeferredRenderer->GetDepthStencil()->GetDepthTexture(), sizeof(ITexture2D*));
	//Texture::CopyData(m_DepthTexture->GetDepthTexture(), m_DeferredRenderer->GetDepthStencil()->GetDepthTexture());


// #if !defined(_PROFILE) && !defined(_FINAL)
// 	WriteDebugTextures();
// #endif


	//VolumeParticles();


	//m_ShadowPass.ProcessShadows(&m_DirectionalShadow, m_RenderContext);

	const CU::Matrix44f& shadow_mvp = m_DirectionalShadow.GetMVP();
	const CU::Matrix44f& camera_orientation = m_Camera->GetOrientation();
	const CU::Matrix44f& camera_projection = m_Camera->GetPerspective();

	m_DeferredRenderer->DeferredRender(camera_orientation, camera_projection, shadow_mvp, m_Direction, m_RenderContext);

	RenderTerrain(false);
	//m_Atmosphere.Render(m_Camera->GetOrientation(), m_DeferredRenderer->GetDepthStencil(), m_RenderContext);

	RenderPointlight();
	RenderSpotlight();

	//m_Engine->ResetRenderTargetAndDepth();
	//m_DeferredRenderer->SetBuffers(m_RenderContext); //This is just the quad

	//m_PostProcessManager.Process(m_DeferredRenderer->GetFinalTexture());
/*
	if (m_PostProcessManager.GetFlags() == 0)
		m_DeferredRenderer->Finalize(m_RenderContext);

	m_API->GetContext()->OMSetRenderTargets(1, m_API->GetBackbufferRef(), m_DeferredRenderer->GetDepthStencil()->GetDepthView());

	m_Engine->ResetRenderTargetAndDepth();

	RenderNonDeferred3DCommands();
*/

	//RenderParticles(m_Engine->GetEffect("Shaders/T_Particle.json"));
	//RenderLines();
	//Render2DCommands();
 
#if !defined(_PROFILE) && !defined(_FINAL)
	//m_Engine->GetEffect("Shaders/debug_ui.json")->Use();
	ImGui::Render();
#endif
// 	m_Engine->Present();

#ifdef _PROFILE
	EASY_BLOCK("Waiting for Logic!");
#endif
	m_Synchronizer->WaitForLogic();
#ifdef _PROFILE
	EASY_END_BLOCK;
#endif
	Engine::GetInstance()->GetMemorySegmentHandle().Clear((s32)m_Synchronizer->GetCurrentBufferIndex());
	m_Synchronizer->SwapBuffer();
	m_Synchronizer->RenderIsDone();

}

#if !defined(_PROFILE) && !defined(_FINAL)
void Renderer::WriteDebugTextures()
{
	float clear[4] = { 0,0,0,0 };
	auto& ctx = m_RenderContext.GetContext();
	ctx.ClearRenderTarget(m_DebugTexture0->GetRenderTargetView(), clear);
	ctx.ClearRenderTarget(m_DebugTexture1->GetRenderTargetView(), clear);
	ctx.ClearRenderTarget(m_DebugTexture2->GetRenderTargetView(), clear);
	ctx.ClearRenderTarget(m_DebugTexture3->GetRenderTargetView(), clear);
	ctx.ClearRenderTarget(m_DebugTexture4->GetRenderTargetView(), clear);

	IRenderTargetView* rtv[] =
	{
		m_DebugTexture0->GetRenderTargetView(),
		m_DebugTexture1->GetRenderTargetView(),
		m_DebugTexture2->GetRenderTargetView(),
		m_DebugTexture3->GetRenderTargetView(),
		m_DebugTexture4->GetRenderTargetView(),
	};
	ctx.OMSetRenderTargets(ARRSIZE(rtv), rtv, nullptr);
	m_DebugQuad->Render(false);
}
#endif

void Renderer::ProcessWater()
{
// 	memcpy(m_WaterCamera, m_Camera, sizeof(Camera)); //This seem extremely unsafe!
// 	Camera* old_camera = m_Camera;
// 	m_Camera = m_WaterCamera;
// 
// 
// 	m_WaterPlane->SetupRefractionRender(m_RenderContext);
// 	m_WaterPlane->SetClipPlane({ 0.f, -1.f, 0.f, 2.f }, m_RenderContext);
// 	RenderTerrain(true);
// 
// 	Render3DCommandsInstanced();
// 
// 	CU::Vector3f position0 = old_camera->GetPosition();
// 	m_Camera->SetPosition(position0);
// 
// 	float distance = 2 * (position0.y - m_WaterPlane->GetPosition().y);
// 	position0.y -= distance;
// 	m_Camera->SetPosition(position0);
// 	m_Camera->InvertAll();
// 	m_WaterPlane->SetupReflectionRender(m_RenderContext);
// 	m_WaterPlane->SetClipPlane({ 0.f, 1.f, 0.f, 2.f }, m_RenderContext);
// 	RenderTerrain(true);
// 
// 	Render3DCommandsInstanced();
// 	m_Atmosphere.Render(m_Camera->GetOrientation(), m_DepthTexture, m_RenderContext);
// 
// 	position0.y += distance;
// 	m_Camera->SetPosition(position0);
// 
// 
// 	m_Camera = old_camera;
}

void Renderer::RenderNonDeferred3DCommands()
{

#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Amber);
#endif
	const auto commands = m_Synchronizer->GetRenderCommands(eBufferType::NO_DEFERRED_BUFFER);
	for (s32 i = 0; i < commands.Size(); i++)
	{
		auto command = reinterpret_cast<ModelCommandNonDeferred*>(commands[i]);
		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::MODEL, "Incorrect command type! Expected MODEL");

		//m_API->SetBlendState(eBlendStates::BLEND_FALSE);
		Model* model = m_RenderContext.GetEngine().GetModel(command->m_Key);
		model->SetOrientation(command->m_Orientation);
		//m_API->SetRasterizer(command->m_Wireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_BACK);
		model->Render(m_Camera->GetOrientation(), m_Camera->GetPerspective(), m_RenderContext);

	}
}

void Renderer::Render3DCommands()
{
#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Green);
#endif
// 	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
// 	m_API->SetBlendState(eBlendStates::BLEND_FALSE);

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

			//m_API->SetBlendState(eBlendStates::BLEND_FALSE);
			Model* model = m_RenderContext.GetEngine().GetModel(command->m_Key);
			model->SetOrientation(command->m_Orientation);
			//m_RenderContext.GetAPI()->SetRasterizer(command->m_Wireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_BACK);
			model->Render(orientation, perspective, m_RenderContext);
		}
	}

#ifdef _PROFILE
	EASY_END_BLOCK;
#endif

}

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

	const CU::Matrix44f& orientation = m_Camera->GetOrientation();
	const CU::Matrix44f& perspective = m_Camera->GetPerspective();
	const u16 current_buffer = Engine::GetInstance()->GetSynchronizer()->GetCurrentBufferIndex();
	for (s32 j = 0; j < 8; j++)
	{
		const auto& commands = Engine::GetInstance()->GetMemorySegmentHandle().GetCommandAllocator(current_buffer, j);
		for (s32 i = 0; i < commands.Size(); i++)
		{
			m_API->SetBlendState(eBlendStates::BLEND_FALSE);
			m_API->SetRasterizer(eRasterizer::CULL_NONE); 
			auto command = reinterpret_cast<ModelCommand*>(commands[i]);
			const bool result = (command->m_CommandType == RenderCommand::MODEL);
			DL_ASSERT_EXP(result == true, "Incorrect command type! Expected MODEL");
			Model* model = m_Engine->GetModel(command->m_Key);
			model->SetPosition(command->m_Orientation.GetPosition());

			m_values.metal = command->m_Metalness;
			m_values.rough = command->m_Roughness;
			m_Engine->GetAPI()->UpdateConstantBuffer(m_PBLValues, &m_values);

			m_RenderContext.GetContext().PSSetConstantBuffer(0, 1, &m_PBLValues);
			model->Render(orientation, perspective, m_RenderContext);
		}
	}

	
 
 	for (auto it = m_ModelsToRender.begin(); it != m_ModelsToRender.end(); it++)
 	{
 		
// 		t per model instance? Array with bools / byte to see if it is wireframe or not?
 		it->second->RenderInstanced(orientation, perspective, m_RenderContext);
 	}
}

void Renderer::RenderTerrain(bool override_effect)
{
// 	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
// 	m_API->SetBlendState(eBlendStates::BLEND_FALSE);
// 	m_API->SetRasterizer(eRasterizer::CULL_BACK);
// #ifdef _PROFILE
// 	EASY_FUNCTION();
// #endif
// 	for (Terrain* terrain : myTerrainArray)
// 	{
// 		if (!terrain->HasLoaded())
// 			continue;
// 		if (!override_effect)
// 			terrain->Render(m_Camera->GetOrientation(), m_Camera->GetPerspective(), m_RenderContext);
// 		else
// 			terrain->Render(m_Camera->GetOrientation(), m_Camera->GetPerspective(), m_RenderContext, true);
// 
// 	}
}

void Renderer::Render3DShadows(const CU::Matrix44f& orientation, Camera* camera)
{
	const auto& commands = m_Synchronizer->GetRenderCommands(eBufferType::MODEL_BUFFER);
	/*m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_API->SetBlendState(eBlendStates::BLEND_FALSE);
	m_API->SetRasterizer(eRasterizer::CULL_NONE);*/

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

void Renderer::Render2DCommands()
{

	m_API->SetRasterizer(eRasterizer::CULL_NONE);
	m_API->SetDepthStencilState(eDepthStencilState::Z_DISABLED, 0);
	m_API->SetBlendState(eBlendStates::NO_BLEND);

	//_________________________
	// RenderSpriteCommands function?
	m_API->SetBlendState(eBlendStates::ALPHA_BLEND);
	const auto commands0 = m_Synchronizer->GetRenderCommands(eBufferType::SPRITE_BUFFER);
	for (s32 i = 0; i < commands0.Size(); i++)
	{
		auto command = reinterpret_cast<SpriteCommand*>(commands0[i]);
		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::SPRITE, "Expected Sprite command type");
		Sprite* sprite = m_Engine->GetSprite(command->m_Key);
		sprite->SetPosition(command->m_Position);
		sprite->Render(m_Camera);
		//mySprite->SetPosition(command->m_Position);
		//mySprite->SetShaderView(command->m_Resource);
		//mySprite->Render(m_Camera);
	}

	//_________________________
	// RenderTextCommands function?
	const auto commands1 = m_Synchronizer->GetRenderCommands(eBufferType::TEXT_BUFFER);
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

void Renderer::RenderSpotlight()
{
#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Purple);
#endif
	Effect* effect = m_LightPass.GetSpotlightEffect();

	SpotlightData data;
	const auto commands = m_Synchronizer->GetRenderCommands(eBufferType::SPOTLIGHT_BUFFER);
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

void Renderer::RenderPointlight()
{
#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Purple);
#endif
	const auto commands = m_Synchronizer->GetRenderCommands(eBufferType::POINTLIGHT_BUFFER);


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

void Renderer::RenderParticles(Effect* effect)
{

	m_API->SetBlendState(eBlendStates::PARTICLE_BLEND);
	m_API->SetRasterizer(eRasterizer::CULL_NONE);
	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 0);

	const auto commands = m_Synchronizer->GetRenderCommands(eBufferType::PARTICLE_BUFFER);
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

	const auto commands = m_Synchronizer->GetRenderCommands(eBufferType::LINE_BUFFER);
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

void Renderer::ProcessCommand(const memory::CommandAllocator& commands, s32 i)
{
	/*model->AddOrientation(command->m_Orientation);
	if (m_ModelsToRender.find(command->m_Key) == m_ModelsToRender.end())
	m_ModelsToRender.emplace(command->m_Key, model);*/
}

//Move this to some kind of light manager
int Renderer::RegisterLight()
{
	SpotLight* s = new SpotLight;
	s->Initiate();
	m_Spotlights.Add(s);
	return (m_Spotlights.Size() - 1);
}

//Terrain Manager not good enough to just loop through?
void Renderer::AddTerrain(Terrain* someTerrain)
{
	myTerrainArray.Add(someTerrain);
}