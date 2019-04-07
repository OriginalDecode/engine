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
#include <Engine/TerrainManager.h>

#include <Engine/RenderNode.h>
#include <Engine/RenderNodeVegetation.h>
#include <Engine/RenderNodeGeneral.h>

#include "shader_types.h"

Renderer::Renderer(Synchronizer* synchronizer)
	: m_Synchronizer(synchronizer)
	, m_Camera(Engine::GetInstance()->GetCamera())
{
	Engine* engine = Engine::GetInstance();
	auto api = Engine::GetAPI();
	m_RenderContext = graphics::RenderContext(Engine::GetInstance(), api->GetDevice(), api->GetContext(), api);

	m_Text = new CText("Data/Font/OpenSans-Regular.ttf", 8, 0);
	//m_Text = new CText("Data/Font/OpenSans-Regular.ttf", 8, 0);
	m_DeferredRenderer = new DeferredRenderer;
	m_GBuffer.Initiate(true);

	Effect* effect = Engine::GetInstance()->GetEffect("Data/Shaders/ssr.json");
	effect->AddShaderResource(m_GBuffer.GetDepth(), TextureSlot::DEPTH);
	effect->AddShaderResource(m_GBuffer.GetNormal(), TextureSlot::NORMAL);
	WindowSize window_size;
	window_size.m_Height = api->GetInfo().m_WindowHeight;
	window_size.m_Width = api->GetInfo().m_WindowWidth;

	m_PostProcessManager.SetPassesToProcess(PostProcessManager::HDR /*| PostProcessManager::SSR*/); //Can be read from a settings file

	m_Line = new Line3D;
	m_Line->Initiate();

	m_LightPass = new graphics_deprecated::LightPass(m_GBuffer);

	m_ParticleEmitter = new CEmitterInstance;
	m_ParticleEmitter->Initiate(m_Synchronizer, m_DepthTexture);


	m_ShadowPass.Initiate(this);
	m_DirectionalShadow.Initiate(2048.f);

	m_Direction = CU::Vector3f(0.0f, 1.0f, 0.0f);

	myPointLight = new PointLight();

	InitiateDebug();

	m_PostProcessManager.Initiate();

	m_PixelBuffer.RegisterVariable(&m_Camera->GetInvProjection());
	m_PixelBuffer.RegisterVariable(&m_Camera->GetPixelOrientation());
	m_PixelBuffer.Initiate("PerFramePixel");

	m_ViewProjection.RegisterVariable(&m_Camera->GetViewProjection());
	m_ViewProjection.Initiate("ViewProj");
	m_WaterCamera = new Camera;
	m_WaterCamera->CreatePerspectiveProjection((float)window_size.m_Width, (float)window_size.m_Height, 0.01f, 100.f, 90.f);

	m_Spotlights.Add(new SpotLight);

	m_RenderContext.GetEngine().LoadEffect("Data/Shaders/wireframe_terrain.json");
	m_TerrainSystem = new TerrainSystem;



	m_Atmosphere.Initiate(1200, 1200, { 512, 0.f, 512.f });
	m_Background = new Quad(Engine::GetInstance()->GetEffect("Shaders/skysphere.json"));

	m_WaterPlane = new WaterPlane;
	

	//m_DeferredRenderer->GetAmbientEffect()->AddShaderResource(m_Background->GetTexture(), TextureSlot::REGISTER_2);

	m_RenderNodes.Add(new graphics::RenderNodeVegetation);
	m_RenderNodes.Add(new graphics::RenderNodeGeneral);

	// m_Text->SetText("The quick brown fox jumps over the lazy dog");
	m_Text->SetText("=>?@ABC");
	m_Text->SetScale({ 5, 5 });
	m_Text->SetPosition({ 0.5f, 0.5f });
}

void Renderer::InitiateDebug()
{
#ifdef _DEBUG
	const graphics::CreateInfo& info = Engine::GetAPI()->GetInfo();
	m_DebugTexture = new Texture;
	m_DebugTexture->InitiateAsRenderTarget(info.m_WindowWidth, info.m_WindowHeight, "diffuse, albedo");
	m_DebugQuad = new Quad(Engine::GetInstance()->GetEffect("Shaders/debug_textures.json"));

	debug::DebugHandle* pDebug = debug::DebugHandle::GetInstance();
	pDebug->SetDebugTexture(m_DebugTexture);
	pDebug->RegisterTexture(m_GBuffer.GetDiffuse(), "albedo");
	pDebug->RegisterTexture(m_GBuffer.GetNormal(), "normal");
	pDebug->RegisterTexture(m_GBuffer.GetDepth(), "depth");
	pDebug->RegisterTexture(m_GBuffer.GetEmissive(), "emissive");
	pDebug->RegisterTexture(m_GBuffer.GetIDTexture(), "entity_id");
	pDebug->RegisterTexture(m_GBuffer.GetRoughness(), "roughness");
	pDebug->RegisterTexture(m_GBuffer.GetMetalness(), "metalness");

#endif
}


Renderer::~Renderer()
{
	delete m_TerrainSystem;

	m_Spotlights.DeleteAll();

#if !defined(_PROFILE) && !defined(_FINAL)
	SAFE_DELETE(m_DebugTexture);
	SAFE_DELETE(m_DebugQuad);
#endif
	SAFE_DELETE(m_Text);

	m_ShadowPass.CleanUp();
	m_DirectionalShadow.CleanUp();

	SAFE_DELETE(m_WaterPlane);
	SAFE_DELETE(m_WaterCamera);
	SAFE_DELETE(m_Line);
	SAFE_DELETE(m_DeferredRenderer);
	SAFE_DELETE(myPointLight);
	SAFE_DELETE(m_LightPass);
	SAFE_DELETE(m_ParticleEmitter);

	m_RenderNodes.DeleteAll();

}

void Renderer::PrepareFrame()
{
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
	m_ViewProjection.Bind(0, EShaderTypeFlag_VERTEX | EShaderTypeFlag_DOMAIN, m_RenderContext);
	m_TerrainSystem->Update();



	m_GBuffer.Clear(clearcolor::black, m_RenderContext);
	m_GBuffer.SetDepthTarget(m_RenderContext);
	
	
	for (graphics::IRenderNode* node : m_RenderNodes)
	{
		node->Draw(m_RenderContext);
	}
	//m_WaterPlane->Render(m_RenderContext);
	m_TerrainSystem->Draw();


	m_GBuffer.SetAsRenderTarget(nullptr, m_RenderContext);

	m_RenderContext.GetContext().SetDepthState(graphics::Z_EQUAL, 1);
	m_TerrainSystem->Draw();
	//m_WaterPlane->Render(m_RenderContext);
	
	for (graphics::IRenderNode* node : m_RenderNodes)
	{
		node->Draw(m_RenderContext);
	}

	DrawIBL();

#ifdef _DEBUG
	WriteDebugTextures();
#endif

	m_RenderContext.GetAPI().SetDefaultTargets();
	m_DeferredRenderer->Finalize();


	if (m_PostProcessManager.GetFlags() != 0)
	{
		m_PostProcessManager.Process(m_DeferredRenderer->GetScene(), m_RenderContext);
	}
	else
	{
		m_RenderContext.GetAPI().SetDefaultTargets();
		m_DeferredRenderer->Finalize();
	}

	m_Line->Render(m_RenderContext);

#if !defined(_PROFILE) && !defined(_FINAL)


	ImGui::Render();
	if (m_CreateCubemaps)
	{
		MakeCubemap({ 512.f, 10.f, 512.f }, 128);
		m_CreateCubemaps = false;
	}

#endif
	m_Text->SetScale({ font_scale, font_scale });
	m_Text->Render(m_RenderContext);

	m_RenderContext.GetAPI().EndFrame();
	m_Synchronizer->WaitForLogic();
	m_Synchronizer->RenderIsDone();

}

void Renderer::DrawIBL()
{
	const CU::Matrix44f shadow_mvp = m_DirectionalShadow.GetMVP();
	m_PixelBuffer.Bind(0, EShaderTypeFlag_PIXEL, m_RenderContext);
	m_DeferredRenderer->Prepare(shadow_mvp, m_Direction, m_RenderContext);

	m_Atmosphere.UpdateBuffer(m_RenderContext, m_Camera);
	auto& ctx = m_RenderContext.GetContext();
	ctx.SetDepthState(graphics::Z_DISABLED, 0);
	m_Background->Render(true);
	m_DeferredRenderer->Draw();


}

#if !defined(_PROFILE) && !defined(_FINAL)
void Renderer::WriteDebugTextures()
{
	Effect* debug_textures = m_RenderContext.GetEngine().GetEffect("Shaders/debug_textures.json");
	const int32 index = debug::DebugHandle::GetInstance()->GetDebugTextureIndex();
	Texture* pTex = debug::DebugHandle::GetInstance()->GetTexture(index);
	debug_textures->AddShaderResource(pTex, TextureSlot::DIFFUSE);
	auto& ctx = m_RenderContext.GetContext();

	ctx.ClearRenderTarget(m_DebugTexture->GetRenderTargetView(), clearcolor::black);
	ctx.OMSetRenderTargets(1, m_DebugTexture->GetRenderTargetRef(), nullptr);
	m_DebugQuad->Render(false);
}
#endif

//Move this to some kind of light manager
int Renderer::RegisterLight()
{
	SpotLight* s = new SpotLight;
	m_Spotlights.Add(s);
	return (m_Spotlights.Size() - 1);
}

graphics::IRenderNode* Renderer::GetNode(uint64 type)
{
	for (graphics::IRenderNode* node : m_RenderNodes)
	{
		if (node->GetNodeType() == type)
		{
			return node;
		}
	}
	return nullptr;
}

#if !defined(_PROFILE) && !defined(_FINAL)
#include "DX11Device.h"
#include "DX11Context.h"
#include <DXTex/DirectXTex.h>

void Renderer::MakeCubemap(CU::Vector3f positon, int32 max_resolution, int32 min_resolution /* = 16 */)
{
	delete m_Cubemap;
	m_Cubemap = nullptr;

	Engine* engine = Engine::GetInstance();
	graphics::IGraphicsAPI* api = Engine::GetAPI();
	graphics::IGraphicsDevice& device = api->GetDevice();
	graphics::IGraphicsContext& ctx = api->GetContext();
	ID3D11Device* _device = static_cast<graphics::DX11Device&>(device).GetDevice();
	ID3D11DeviceContext* _ctx = nullptr;
	_device->GetImmediateContext(&_ctx);

	Camera* camera = new Camera;
	const float far_plane = 100000.f;
	const float near_plane = 0.1f;
	const float fov = 90.f;
	camera->CreatePerspectiveProjection(max_resolution, max_resolution, near_plane, far_plane, fov);
	camera->SetPosition(positon);

	TextureDesc depth_desc;
	depth_desc.m_Usage = graphics::DEFAULT_USAGE;
	depth_desc.m_ResourceTypeBinding = graphics::BIND_SHADER_RESOURCE | graphics::BIND_DEPTH_STENCIL /*should bind a render target*/;
	depth_desc.m_TextureFormat = graphics::R32_TYPELESS;
	depth_desc.m_DepthTextureFormat = graphics::DEPTH_32_FLOAT;
	depth_desc.m_Width = max_resolution;
	depth_desc.m_Height = max_resolution;

	Texture* depth = nullptr;
	depth = new Texture;
	depth->Initiate(depth_desc, "");

	int32 downsample_amount = int32(log(__min(max_resolution, max_resolution)) / log(2.f)) + 1;


	graphics::Viewport* viewport = api->CreateViewport(max_resolution, max_resolution, 0, 1, 0, 0);
	ctx.SetViewport(viewport);
	//forward, right, back, left | up, down,
	TextureDesc texDesc;
	texDesc.m_Usage = graphics::DEFAULT_USAGE;
	texDesc.m_ResourceTypeBinding = graphics::BIND_SHADER_RESOURCE | graphics::BIND_RENDER_TARGET;
	texDesc.m_ShaderResourceFormat = graphics::RGBA16_FLOAT;
	texDesc.m_RenderTargetFormat = graphics::RGBA16_FLOAT;
	texDesc.m_TextureFormat = graphics::RGBA16_FLOAT;
	texDesc.m_MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	texDesc.m_MipCount = downsample_amount;
	texDesc.m_Width = max_resolution;
	texDesc.m_Height = max_resolution;

	TerrainManager* manager = Engine::GetInstance()->GetTerrainManager();

	graphics::ConstantBuffer buffer;
	buffer.RegisterVariable(&camera->GetViewProjection());
	buffer.Initiate();

	graphics::ConstantBuffer terrain_buffer;
	Terrain* terrain = manager->GetTerrain(cl::Hash("2048"));
	terrain_buffer.RegisterVariable(&terrain->GetOrientation());
	terrain_buffer.RegisterVariable(&camera->GetOrientation());
	terrain_buffer.Initiate();


	graphics::ConstantBuffer pixel_buffer;
	pixel_buffer.RegisterVariable(&camera->GetInvProjection());
	pixel_buffer.RegisterVariable(&camera->GetPixelOrientation());
	pixel_buffer.Initiate();

	enum : int32
	{
		RIGHT,
		LEFT,
		UP,
		DOWN,
		FORWARD,
		BACK,
		NOF_SIDES
	};


	Texture* cubemap[NOF_SIDES];
	int32 flags = EShaderTypeFlag_VERTEX | EShaderTypeFlag_DOMAIN;




	auto create_texture = [&](int32 index) {
		Texture* rendertarget = new Texture;
		rendertarget->Initiate(texDesc, false, "");
		
		ctx.ClearDepthStencilView(depth->GetDepthView(), graphics::DEPTH | graphics::STENCIL, 1);
		ctx.ClearRenderTarget(rendertarget, clearcolor::black);
		ctx.OMSetRenderTargets(1, rendertarget->GetRenderTargetRef(), depth->GetDepthView());

		camera->Update();
		buffer.Bind(0, flags, m_RenderContext);
		terrain_buffer.Bind(1, flags, m_RenderContext);
		pixel_buffer.Bind(0, EShaderTypeFlag_PIXEL, m_RenderContext);


		ctx.SetDepthState(graphics::Z_DISABLED, 1);
		m_Atmosphere.UpdateBuffer(m_RenderContext, camera);
		m_Background->Render(true);

		terrain->Render(m_RenderContext, false, true);

		_ctx->GenerateMips((ID3D11ShaderResourceView*)rendertarget->GetShaderView());

		cubemap[index] = rendertarget;
	};


	create_texture(FORWARD);
	camera->RotateAroundY(cl::DegreeToRad(90.f));
	create_texture(RIGHT);
	camera->RotateAroundY(cl::DegreeToRad(90.f));
	create_texture(BACK);
	camera->RotateAroundY(cl::DegreeToRad(90.f));
	create_texture(LEFT);
	camera->RotateAroundY(cl::DegreeToRad(90.f));
	camera->RotateAroundX(cl::DegreeToRad(-90.f));
	create_texture(UP);
	camera->RotateAroundX(cl::DegreeToRad(180.f));
	create_texture(DOWN);


	for (int32 i = 0; i < NOF_SIDES; ++i)
	{
		_ctx->GenerateMips((ID3D11ShaderResourceView*)cubemap[i]->GetShaderView());
	}

	m_Cubemap = new Texture;
	m_Cubemap->CreateTextureArray(cubemap, NOF_SIDES, "atmosphere.dds");
	engine->GetEffect("Shaders/deferred_ambient.json")->AddShaderResource(m_Cubemap, TextureSlot::CUBEMAP);


	_ctx->Release();
	delete camera;
	camera = nullptr;
	delete viewport;
	viewport = nullptr;
	delete depth;
	depth = nullptr;

	for (Texture* t : cubemap)
	{
		delete t;
		t = nullptr;
	}

}


#endif
