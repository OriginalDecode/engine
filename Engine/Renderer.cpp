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


#include <Input/InputHandle.h>
#include <Input/InputWrapper.h>

#include <Engine/IGraphicsContext.h>
#include <Engine/DX11Context.h>
#include <Engine/DX11Device.h>
#include <Engine/WaterPlane.h>

#if !defined(_PROFILE) && !defined(_FINAL)
#include <EntitySystem/TranslationComponent.h>
#include <EntitySystem/GraphicsComponent.h>
#include <imgui/imgui_impl_dx11.h>
#endif

#include <Engine/TerrainSystem.h>
#include <Engine/Quad.h>

Renderer::Renderer(Synchronizer* synchronizer)
	: m_Synchronizer(synchronizer)
	, m_Camera(Engine::GetInstance()->GetCamera())
{
	auto api = Engine::GetAPI();
	m_RenderContext = graphics::RenderContext(Engine::GetInstance(), api->GetDevice(), api->GetContext(), api);

	m_Text = new CText("Data/Font/OpenSans-Regular.ttf", 12, 0);
	m_DeferredRenderer = new DeferredRenderer;
	m_GBuffer.Initiate(true);


	Effect* effect = Engine::GetInstance()->GetEffect("Data/Shaders/ssr.json");
	effect->AddShaderResource(m_GBuffer.GetDepth(), Effect::DEPTH);
	effect->AddShaderResource(m_GBuffer.GetNormal(), Effect::NORMAL);
	WindowSize window_size;
	window_size.m_Height = api->GetInfo().m_WindowHeight;
	window_size.m_Width = api->GetInfo().m_WindowWidth;

	//m_DepthTexture = new Texture; //Where should this live?
	//m_DepthTexture->InitiateAsDepthStencil(window_size.m_Width, window_size.m_Height, "Renderer : Depth");

	m_PostProcessManager.SetPassesToProcess(PostProcessManager::HDR /*| PostProcessManager::SSR*/); //Can be read from a settings file

	m_Line = new Line3D; //Where should this live?
	m_Line->Initiate();

	m_LightPass = new graphics::LightPass(m_GBuffer);

	m_ParticleEmitter = new CEmitterInstance;
	m_ParticleEmitter->Initiate(m_Synchronizer, m_DepthTexture);

	m_Atmosphere.Initiate(2048, 2048, { 1024, -128.f, 1024.f });

// 	m_ShadowPass.Initiate(this);
// 
// 
// 	m_DirectionalShadow.Initiate(2048.f);

	m_Direction = CU::Vector3f(0.0f, 1.0f, 0.0f);

	myPointLight = new PointLight();

#if !defined(_PROFILE) && !defined(_FINAL)

	TextureDesc desc;
	desc.m_Width = window_size.m_Width;
	desc.m_Height = window_size.m_Height;
	desc.m_Usage = graphics::DEFAULT_USAGE;
	desc.m_ResourceTypeBinding = graphics::BIND_SHADER_RESOURCE | graphics::BIND_RENDER_TARGET;
	desc.m_ShaderResourceFormat = graphics::RGBA16_FLOAT;
	desc.m_RenderTargetFormat = graphics::RGBA16_FLOAT;
	desc.m_TextureFormat = graphics::RGBA16_FLOAT;
	m_HoverTexture = new Texture;
	m_HoverTexture->Initiate(desc, "HoverTexture");
	m_RenderHoverEffect = Engine::GetInstance()->GetEffect("Shaders/hover.json");
	m_SelectedEffect = Engine::GetInstance()->GetEffect("Shaders/selected.json");

	m_SelectedTexture = new Texture;
	m_SelectedTexture->Initiate(desc, "SelectedTexture");

	const char* names[] = {
		"diffuse, albedo",
		"normal",
		"depth",
		"emissive",
		"entity_id",
		"hover",
		"edge"
	};

	m_DebugTexture = new Texture;
	m_DebugTexture->InitiateAsRenderTarget(window_size.m_Width, window_size.m_Height, "diffuse, albedo");
	m_DebugQuad = new Quad(Engine::GetInstance()->GetEffect("Shaders/debug_textures.json"));

	debug::DebugHandle* pDebug = debug::DebugHandle::GetInstance();
	pDebug->SetDebugTexture(m_DebugTexture);
	pDebug->RegisterTexture(m_GBuffer.GetDiffuse(), names[0]);
	pDebug->RegisterTexture(m_GBuffer.GetNormal(), names[1]);
	pDebug->RegisterTexture(m_GBuffer.GetDepth(), names[2]);
	pDebug->RegisterTexture(m_GBuffer.GetEmissive(), names[3]);
	pDebug->RegisterTexture(m_GBuffer.GetIDTexture(), names[4]);
	pDebug->RegisterTexture(m_HoverTexture, names[5]);
	pDebug->RegisterTexture(m_SelectedTexture, names[6]);

#endif
	m_PostProcessManager.Initiate();

	CU::Matrix44f& inv = m_Camera->GetInvProjection();

	m_PixelBuffer.RegisterVariable(&m_Camera->GetInvProjection());
	m_PixelBuffer.RegisterVariable(&m_Camera->GetOrientation());
	m_PixelBuffer.Initiate("PerFramePixel");

	m_ViewProjection.RegisterVariable(&m_Camera->GetViewProjection());
	m_ViewProjection.Initiate("ViewProj");
	m_WaterCamera = new Camera;
	m_WaterCamera->CreatePerspectiveProjection((float)window_size.m_Width, (float)window_size.m_Height, 0.01f, 100.f, 90.f);

	m_Spotlights.Add(new SpotLight);



	//m_TestTerrain = new Terrain(2048.f);
	//m_RenderContext.GetEngine().LoadEffect("Data/Shaders/wireframe_terrain.json");
	//m_TerrainSystem = new TerrainSystem;

}

Renderer::~Renderer()
{
	delete m_TestTerrain;
	delete m_TerrainSystem;

	m_Spotlights.DeleteAll();

#if !defined(_PROFILE) && !defined(_FINAL)
	SAFE_DELETE(m_DebugTexture);
	SAFE_DELETE(m_DebugQuad);
	SAFE_DELETE(m_HoverTexture);
	SAFE_DELETE(m_SelectedTexture);
#endif
	SAFE_DELETE(m_Text);

	/*m_ShadowPass.CleanUp();
	m_DirectionalShadow.CleanUp();*/

	//SAFE_DELETE(m_WaterPlane);
	SAFE_DELETE(m_WaterCamera);
	SAFE_DELETE(m_Line);
	//SAFE_DELETE(m_DepthTexture);
	SAFE_DELETE(m_DeferredRenderer);
	SAFE_DELETE(myPointLight);
	SAFE_DELETE(m_LightPass);
	SAFE_DELETE(m_ParticleEmitter);

}

void Renderer::Render()
{
	if (m_Synchronizer->HasQuit())
	{
		m_Synchronizer->RenderIsDone();
		return;
	}

	PROFILE_FUNCTION(profiler::colors::Magenta);

	m_RenderContext.GetAPI().BeginFrame();
	m_RenderContext.GetAPI().ResetViewport();

	graphics::IGraphicsContext& ctx = m_RenderContext.GetContext();
	graphics::IGraphicsAPI& api = m_RenderContext.GetAPI();

	const CU::Matrix44f& camera_orientation = m_Camera->GetOrientation();
	const CU::Matrix44f& camera_projection = m_Camera->GetPerspective();
	CU::Matrix44f camera_view_proj = CU::Math::Inverse(camera_orientation) * camera_projection;


	m_ViewProjection.Bind(0, graphics::ConstantBuffer::VERTEX, m_RenderContext);
	m_RenderInstanced = false;
	if (m_RenderInstanced)
		Render3DCommandsInstanced();

	m_GBuffer.Clear(clearcolor::black, m_RenderContext);
	m_GBuffer.SetAsRenderTarget(nullptr, m_RenderContext);

	//m_TerrainSystem->Update();
 
 	Render3DCommands();
 	m_InstancingManager.DoInstancing(m_RenderContext, false);
 
 #if !defined(_PROFILE) && !defined(_FINAL)
 	WriteDebugTextures();
 
 	const Entity hovered = debug::DebugHandle::GetInstance()->GetHoveredEntity();
 	DrawEntity(m_HoverTexture, hovered, ctx);
 
 	const Entity selected = debug::DebugHandle::GetInstance()->GetSelectedEntity();
 	DrawEntity(m_SelectedTexture, selected, ctx);
 
 #endif
 
 	//m_ShadowPass.ProcessShadows(&m_DirectionalShadow);
 
	const CU::Matrix44f shadow_mvp;// = m_DirectionalShadow.GetMVP();
 	m_PixelBuffer.Bind(0, graphics::ConstantBuffer::PIXEL, m_RenderContext);
 	m_DeferredRenderer->DeferredRender(shadow_mvp, m_Direction, m_RenderContext);
 
 	m_ViewProjection.Bind(0, graphics::ConstantBuffer::VERTEX | graphics::ConstantBuffer::GEOMETRY | graphics::ConstantBuffer::DOMAINS, m_RenderContext);
 	m_Atmosphere.Render(m_RenderContext);
 
	m_PixelBuffer.Bind(0, graphics::ConstantBuffer::PIXEL, m_RenderContext);
 	RenderSpotlight();
 	RenderPointlight();
 
	IRenderTargetView* pRenderTarget = m_DeferredRenderer->GetScene()->GetRenderTargetView();
	IDepthStencilView* pDepthStencil = m_RenderContext.GetAPI().GetDepthView();
	m_RenderContext.GetContext().OMSetRenderTargets(1, &pRenderTarget, pDepthStencil);
	//m_DeferredRenderer->GetDepthStencil();
 	RenderParticles(nullptr);
  
  	if (m_PostProcessManager.GetFlags() != 0)
  	{
  		m_PostProcessManager.Process(m_DeferredRenderer->GetScene(), m_RenderContext);
  	}
  	else
  	{
		m_RenderContext.GetAPI().SetDefaultTargets();
		m_DeferredRenderer->Finalize();
  	}
 
 #if !defined(_PROFILE) && !defined(_FINAL)
 	//Detect edges on specified texture
 	//m_PostProcessManager.Process(m_HoverTexture, PostProcessManager::EDGE_DETECTION, m_RenderContext);
 	//m_PostProcessManager.Process(m_SelectedTexture, PostProcessManager::EDGE_DETECTION, m_RenderContext);
 #endif
 
 	m_ViewProjection.Bind(0, graphics::ConstantBuffer::VERTEX, m_RenderContext);
 	RenderLines();
 
 	Render2DCommands();

#if !defined(_PROFILE) && !defined(_FINAL)
	ImGui::Render();
#endif
	m_RenderContext.GetAPI().EndFrame();
	m_InstancingManager.EndFrame();


	m_Synchronizer->WaitForLogic();

	Engine::GetInstance()->GetMemorySegmentHandle().Clear((s32)m_Synchronizer->GetCurrentBufferIndex());
	m_Synchronizer->SwapBuffer();
	m_Synchronizer->RenderIsDone();

}

#if !defined(_PROFILE) && !defined(_FINAL)

static CU::Matrix44f prev;
static CU::Matrix44f orientation;

void Renderer::DrawEntity(Texture* pTex, Entity e, graphics::IGraphicsContext &ctx)
{
	return;
	if (e <= 0)
		return;
	
	ctx.ClearRenderTarget(pTex->GetRenderTargetView(), clearcolor::black);
	ctx.OMSetRenderTargets(1, pTex->GetRenderTargetRef(), nullptr);
	Engine& engine = m_RenderContext.GetEngine();
	if (!engine.GetEntityManager().HasComponent<GraphicsComponent>(e))
		return;
	const GraphicsComponent& graphics = engine.GetEntityManager().GetComponent<GraphicsComponent>(e);

	for (const ModelInstance& instance : graphics.m_Instances)
	{
		m_HoverModel = engine.GetModel<Model>(instance.m_ModelID);
		if (!m_HoverModel.GetData())
			continue;

		const TranslationComponent& translation = engine.GetEntityManager().GetComponent<TranslationComponent>(e);

		
		prev = orientation;
		orientation = translation.GetOrientation();


		const CU::Matrix44f relative = CU::Matrix44f::CreateScaleMatrix(instance.m_Scale)  * instance.m_Orientation;

		m_HoverModel->AddOrientation(relative * prev);
		m_HoverModel->RenderInstanced(m_RenderContext, m_RenderHoverEffect);

	}
}

void Renderer::WriteDebugTextures()
{
	Effect* debug_textures = m_RenderContext.GetEngine().GetEffect("Shaders/debug_textures.json");
	const s32 index = debug::DebugHandle::GetInstance()->GetDebugTextureIndex();
	Texture* pTex = debug::DebugHandle::GetInstance()->GetTexture(index);
	debug_textures->AddShaderResource(pTex, Effect::DIFFUSE);
	auto& ctx = m_RenderContext.GetContext();

	ctx.ClearRenderTarget(m_DebugTexture->GetRenderTargetView(), clearcolor::black);
	ctx.OMSetRenderTargets(1, m_DebugTexture->GetRenderTargetRef(), nullptr);
	m_DebugQuad->Render(false);
}
#endif

void Renderer::ProcessWater()
{
	memcpy(m_WaterCamera, m_Camera, sizeof(Camera)); //This seem extremely unsafe!
	Camera* old_camera = m_Camera;
	m_Camera = m_WaterCamera;

	m_WaterPlane->SetupRefractionRender(m_RenderContext);
	m_WaterPlane->SetClipPlane({ 0.f, -1.f, 0.f, 2.f }, m_RenderContext);
	m_InstancingManager.DoInstancing(m_RenderContext, false);

	CU::Vector3f position0 = old_camera->GetPosition();
	m_Camera->SetPosition(position0);

	float distance = 2 * (position0.y - m_WaterPlane->GetPosition().y);
	position0.y -= distance;
	m_Camera->SetPosition(position0);
	m_Camera->InvertAll();
	m_WaterPlane->SetupReflectionRender(m_RenderContext);
	m_WaterPlane->SetClipPlane({ 0.f, 1.f, 0.f, 2.f }, m_RenderContext);
	m_InstancingManager.DoInstancing(m_RenderContext, false);

	position0.y += distance;
	m_Camera->SetPosition(position0);

	m_Camera = old_camera;
}

void Renderer::RenderNonDeferred3DCommands()
{

	PROFILE_FUNCTION(profiler::colors::Amber);
	const auto commands = m_Synchronizer->GetRenderCommands(eBufferType::NO_DEFERRED_BUFFER);
	for (s32 i = 0; i < commands.Size(); i++)
	{
		auto command = reinterpret_cast<ModelCommandNonDeferred*>(commands[i]);
		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::MODEL, "Incorrect command type! Expected MODEL");

		//m_API->SetBlendState(eBlendStates::BLEND_FALSE);
		RefPointer<Model> model = m_RenderContext.GetEngine().GetModel<Model>(command->m_Key);
		model->SetOrientation(command->m_Orientation);
		//m_API->SetRasterizer(command->m_Wireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_BACK);
		model->Render(m_RenderContext);

	}
}

void Renderer::Render3DCommands()
{
	PROFILE_FUNCTION(profiler::colors::Green);

	graphics::IGraphicsAPI& api = m_RenderContext.GetAPI();
	graphics::IGraphicsContext& ctx = m_RenderContext.GetContext();

	ctx.PSSetSamplerState(0, 1, graphics::LINEAR_WRAP);
	ctx.SetDepthState(api.GetDepthStencilState(graphics::Z_ENABLED), 1);
	ctx.SetRasterizerState(api.GetRasterizerState(graphics::CULL_BACK));
	ctx.SetBlendState(api.GetBlendState(graphics::BLEND_FALSE));

	const u16 current_buffer = Engine::GetInstance()->GetSynchronizer()->GetCurrentBufferIndex();
	for (s32 j = 0; j < 8; j++)
	{
		const auto& commands = Engine::GetInstance()->GetMemorySegmentHandle().GetCommandAllocator(current_buffer, j);

		for (s32 i = 0; i < commands.Size(); i++)
		{
			auto command = reinterpret_cast<ModelCommand*>(commands[i]);
			DL_ASSERT_EXP(command->m_CommandType == RenderCommand::MODEL, "Incorrect command type! Expected MODEL");
			RefPointer<Model> model = m_RenderContext.GetEngine().GetModel<Model>(command->m_Key);
			if (model->IsInstanced())
			{
				ProcessModelCommand(commands, i, m_RenderContext.GetEngine());
				continue;
			}

#ifdef _DEBUG
			model->SetEntityID(command->m_EntityID);
#endif
			model->SetOrientation(command->m_Orientation);
			model->Render(m_RenderContext);
		}
	}
}

void Renderer::Render3DCommandsInstanced()
{
	PROFILE_FUNCTION(profiler::colors::Green);
	const u16 current_buffer = Engine::GetInstance()->GetSynchronizer()->GetCurrentBufferIndex();
	Engine& engine = m_RenderContext.GetEngine();
	const memory::MemorySegmentHandle& mem_handle = engine.GetMemorySegmentHandle();
	for (s32 top_tree_node = 0; top_tree_node < 8; top_tree_node++)
	{
		const auto& commands = mem_handle.GetCommandAllocator(current_buffer, top_tree_node);
		for (s32 i = 0; i < commands.Size(); i++)
		{
			ProcessModelCommand(commands, i, engine);
		}
	}

}

void Renderer::RenderTerrain(bool /*override_effect*/)
{
	graphics::IGraphicsContext& ctx = m_RenderContext.GetContext();
	graphics::IGraphicsAPI& api = m_RenderContext.GetAPI();

	ctx.SetDepthState(api.GetDepthStencilState(graphics::Z_ENABLED), 1);
#ifdef _DEBUG
	ctx.SetRasterizerState(terrainWireframe ? api.GetRasterizerState(graphics::WIREFRAME) : api.GetRasterizerState(graphics::CULL_BACK));
#else
	ctx.SetRasterizerState(api.GetRasterizerState(graphics::CULL_BACK));
#endif
	ctx.SetBlendState(api.GetBlendState(graphics::BLEND_FALSE));
	PROFILE_FUNCTION(profiler::colors::Green);
	for (Terrain* terrain : myTerrainArray)
	{
		if (!terrain->HasLoaded())
			continue;

		terrain->Render(m_RenderContext);
	}
}

void Renderer::Render3DShadows(const CU::Matrix44f&, Camera* camera)
{
// 	const CU::Matrix44f& perspective = camera->GetPerspective();
// 	const CU::Matrix44f& orientation = camera->GetCurrentOrientation();
// 	const CU::Matrix44f& view_proj = CU::Math::Inverse(orientation) * perspective;
// 	m_RenderContext.GetContext().UpdateConstantBuffer(m_ViewProjBuffer, &view_proj, sizeof(CU::Matrix44f));

	const u16 current_buffer = Engine::GetInstance()->GetSynchronizer()->GetCurrentBufferIndex();
	graphics::IGraphicsAPI& api = m_RenderContext.GetAPI();
	graphics::IGraphicsContext& ctx = m_RenderContext.GetContext();
	Engine& engine = m_RenderContext.GetEngine();

	ctx.SetDepthState(api.GetDepthStencilState(graphics::Z_ENABLED), 1);
	ctx.SetRasterizerState(api.GetRasterizerState(graphics::CULL_NONE));
	ctx.SetBlendState(api.GetBlendState(graphics::BLEND_FALSE));
	for (s32 top_tree_node = 0; top_tree_node < 8; top_tree_node++)
	{
		const auto& commands = Engine::GetInstance()->GetMemorySegmentHandle().GetCommandAllocator(current_buffer, top_tree_node);
		for (s32 i = 0; i < commands.Size(); i++)
		{
			ProcessModelCommand(commands, i, engine);
		}
	}

	m_InstancingManager.DoInstancing(m_RenderContext, true);

}

void Renderer::Render2DCommands()
{
	PROFILE_FUNCTION(profiler::colors::Red);
	graphics::IGraphicsAPI& api = m_RenderContext.GetAPI();
	graphics::IGraphicsContext& ctx = m_RenderContext.GetContext();


	IDepthStencilState* dss = m_RenderContext.GetAPI().GetDepthStencilState(graphics::Z_DISABLED);
	IRasterizerState* rss = m_RenderContext.GetAPI().GetRasterizerState(graphics::CULL_NONE);
	m_RenderContext.GetContext().SetDepthState(dss, 0);
	m_RenderContext.GetContext().SetRasterizerState(rss);
	const auto commands = m_Synchronizer->GetRenderCommands(eBufferType::TEXT_BUFFER);
	for (s32 i = 0; i < commands.Size(); i++)
	{
		auto command = reinterpret_cast<TextCommand*>(commands[i]);
		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::TEXT, "Expected Text command type");
		m_Text->SetText(command->m_TextBuffer);
		m_Text->SetPosition(command->m_Position);
		m_Text->Render(m_RenderContext);
	}
	
	
	//m_Text->Render(m_Camera);

	// 	m_API->SetRasterizer(eRasterizer::CULL_NONE);
	// 	m_API->SetDepthStencilState(eDepthStencilState::Z_DISABLED, 0);
	// 	m_API->SetBlendState(eBlendStates::NO_BLEND);
	// 
	// 	//_________________________
	// 	// RenderSpriteCommands function?
	// 	m_API->SetBlendState(eBlendStates::ALPHA_BLEND);
	// 	const auto commands0 = m_Synchronizer->GetRenderCommands(eBufferType::SPRITE_BUFFER);
	// 	for (s32 i = 0; i < commands0.Size(); i++)
	// 	{
	// 		auto command = reinterpret_cast<SpriteCommand*>(commands0[i]);
	// 		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::SPRITE, "Expected Sprite command type");
	// 		Sprite* sprite = m_Engine->GetSprite(command->m_Key);
	// 		sprite->SetPosition(command->m_Position);
	// 		sprite->Render(m_Camera);
	// 		//mySprite->SetPosition(command->m_Position);
	// 		//mySprite->SetShaderView(command->m_Resource);
	// 		//mySprite->Render(m_Camera);
	// 	}
	// 
	// 	//_________________________
	// 	// RenderTextCommands function?
	// 	const auto commands1 = m_Synchronizer->GetRenderCommands(eBufferType::TEXT_BUFFER);
	// 	for (s32 i = 0; i < commands1.Size(); i++)
	// 	{
	// 		auto command = reinterpret_cast<TextCommand*>(commands1[i]);
	// 		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::TEXT, "Expected Text command type");
	// 		myText->SetText(command->m_TextBuffer);
	// 		myText->SetPosition(command->m_Position);
	// 		myText->Render(m_Camera);
	// 	}
	// 
	// 	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	// 	m_API->SetRasterizer(eRasterizer::CULL_BACK);

}

void Renderer::RenderSpotlight()
{

	PROFILE_FUNCTION(profiler::colors::Purple);

	SpotlightData data;
	const auto commands = m_Synchronizer->GetRenderCommands(eBufferType::SPOTLIGHT_BUFFER);

	PROFILE_BLOCK("Spotlight Command Loop", profiler::colors::Red);

	for (s32 i = 0; i < commands.Size(); i++)
	{
		auto command = reinterpret_cast<SpotlightCommand*>(commands[i]);
		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::SPOTLIGHT, "Expected Spotlight command type");

		data.myAngle = command->m_Angle;
		data.myRange = command->m_Range;
		data.myLightColor = command->m_Color;
		data.myLightPosition = command->m_Orientation.GetPosition();

		data.myOrientation = command->m_Orientation; //I don't want this matrix to be scaled since it would have strange implications on how light is handled.

		data.m_Intensity = command->m_Intensity;

		SpotLight* light = m_Spotlights[command->m_LightID];
		light->SetData(data);

		CU::Matrix44f shadow_mvp;

		m_RenderContext.GetContext().SetRasterizerState(m_RenderContext.GetAPI().GetRasterizerState(graphics::CULL_NONE));
		m_LightPass->RenderSpotlight(light, m_Camera->GetOrientation(), m_Camera->GetPerspective(), shadow_mvp, m_RenderContext);

	}
	PROFILE_BLOCK_END;
}

void Renderer::RenderPointlight()
{
	PROFILE_FUNCTION(profiler::colors::Purple);
	const auto commands = m_Synchronizer->GetRenderCommands(eBufferType::POINTLIGHT_BUFFER);

	PROFILE_BLOCK("Pointlight Command Loop", profiler::colors::Red);
	for (s32 i = 0; i < commands.Size(); i++)
	{
		auto command = reinterpret_cast<PointlightCommand*>(commands[i]);

		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::POINTLIGHT, "Wrong command type in pointlight buffer.");
		myPointLight->SetPosition(command->m_Orientation.GetPosition());
		myPointLight->SetRange(command->m_Range);
		//myPointLight->SetRange(debug::DebugHandle::GetInstance()->m_Range);
		myPointLight->SetColor(CU::Vector4f(command->m_Color.x, command->m_Color.y, command->m_Color.z, 1));
		myPointLight->Update();
		CU::Matrix44f shadow_mvp;
#if !defined(_FINAL) && !defined(_PROFILE)
		m_RenderContext.GetContext().SetRasterizerState(m_RenderContext.GetAPI().GetRasterizerState(m_LightModelWireframe ? graphics::WIREFRAME : graphics::CULL_NONE));
#else
		m_RenderContext.GetContext().SetRasterizerState(m_RenderContext.GetAPI().GetRasterizerState(graphics::CULL_NONE));
#endif
		m_LightPass->RenderPointlight(myPointLight, m_Camera->GetOrientation(), m_Camera->GetOrientation(), shadow_mvp, m_RenderContext);
	}
	PROFILE_BLOCK_END;
}

void Renderer::RenderParticles(Effect* effect)
{
	m_RenderContext.GetContext().SetBlendState(m_RenderContext.GetAPI().GetBlendState(graphics::PARTICLE_BLEND));
	const auto commands = m_Synchronizer->GetRenderCommands(eBufferType::PARTICLE_BUFFER);
	for (s32 i = 0; i < commands.Size(); i++)
	{
		auto command = reinterpret_cast<ParticleCommand*>(commands[i]);
		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::PARTICLE, "Expected particle command type");
		m_ParticleEmitter->SetPosition(command->m_Position);

		m_ParticleEmitter->Update(m_RenderContext.GetEngine().GetDeltaTime());

		m_RenderContext.GetContext().SetRasterizerState(m_RenderContext.GetAPI().GetRasterizerState(graphics::CULL_NONE));
		m_ParticleEmitter->Render(m_Camera->GetOrientation(), m_Camera->GetPerspective(), effect);
	}
}

void Renderer::RenderLines()
{

	PROFILE_FUNCTION(profiler::colors::Amber);
	const auto commands = m_Synchronizer->GetRenderCommands(eBufferType::LINE_BUFFER);
	PROFILE_BLOCK("Line Command Loop", profiler::colors::Red);


	for (s32 i = 0; i < commands.Size(); i++)
	{
		auto command = reinterpret_cast<LineCommand*>(commands[i]);
		const bool result = command->m_CommandType == RenderCommand::LINE;
		if (!result)
			return;
		DL_ASSERT_EXP(command->m_CommandType == RenderCommand::LINE, "Expected Line command type");
		m_Line->AddLine(command->m_Points);
	}
	m_Line->Render(m_RenderContext);
	PROFILE_BLOCK_END;
}

void Renderer::ProcessModelCommand(const memory::CommandAllocator& commands, s32 i, Engine& engine)
{
	auto command = reinterpret_cast<ModelCommand*>(commands[i]);
	const bool result = (command->m_CommandType == RenderCommand::MODEL);
	DL_ASSERT_EXP(result == true, "Incorrect command type! Expected MODEL");

	RefPointer<Model> model = engine.GetModel<Model>(command->m_Key);
	Material* material = model->GetMaterial() ? model->GetMaterial() : engine.GetMaterial(command->m_MaterialKey);
	if (!material)
		return;

	const u64 key = material->GetKey();


	if (!m_InstancingManager.FindInstanceObject(key))
	{
		InstanceObject new_instance;
		new_instance.m_Material = material;
		new_instance.m_Model = model;
		new_instance.m_Shadowed = true; /* should be command->m_Shadowed or something*/
		m_InstancingManager.AddInstanceObject(new_instance);
	}
	GPUModelData model_data;
	model_data.m_Orientation = command->m_Orientation;

#ifdef _DEBUG
	model_data.m_ID = command->m_EntityID;
	model_data.m_Hovering = (command->m_EntityID == debug::DebugHandle::GetInstance()->GetHoveredEntity() ? 1 : 0);
#endif
	m_InstancingManager.AddGPUDataToInstance(key, model->GetKey(), model_data);
}

//Move this to some kind of light manager
int Renderer::RegisterLight()
{
	SpotLight* s = new SpotLight;
	m_Spotlights.Add(s);
	return (m_Spotlights.Size() - 1);
}

//Terrain Manager not good enough to just loop through?
void Renderer::AddTerrain(Terrain* someTerrain)
{
	myTerrainArray.Add(someTerrain);
}