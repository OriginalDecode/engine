#include "stdafx.h"
#include "DeferredRenderer.h"
#include "PointLight.h"
#include <DL_Debug.h>
#include "GBuffer.h"

#include <Engine/Quad.h>
#include <Engine/IGraphicsAPI.h>
#include <Engine/IGraphicsDevice.h>
#include <Engine/IGraphicsContext.h>


DeferredRenderer::DeferredRenderer()
{
	const graphics::IGraphicsAPI* api = Engine::GetAPI();
	WindowSize window_size;
	window_size.m_Height = api->GetInfo().m_WindowHeight;
	window_size.m_Width = api->GetInfo().m_WindowWidth;


	const char* cube[] = {
		"Data/Textures/cube/miramar_lf.dds",
		"Data/Textures/cube/miramar_rt.dds",
		"Data/Textures/cube/miramar_up.dds",
		"Data/Textures/cube/miramar_dn.dds",
		"Data/Textures/cube/miramar_ft.dds",
		"Data/Textures/cube/miramar_bk.dds",
	};

	constexpr size_t texCount = ARRSIZE(cube);

	Texture* texCube = new Texture;
	texCube->InitiateTextureArray(cube, texCount, "Array");

	//_______________________________________________________________________

	TextureDesc scene_desc;
	scene_desc.m_Width = window_size.m_Width;
	scene_desc.m_Height = window_size.m_Height;
	scene_desc.m_Usage = graphics::DEFAULT_USAGE;
	scene_desc.m_ResourceTypeBinding = graphics::BIND_SHADER_RESOURCE | graphics::BIND_RENDER_TARGET;
	scene_desc.m_TextureFormat = graphics::RGBA16_FLOAT;
	scene_desc.m_ShaderResourceFormat = graphics::RGBA16_FLOAT;
	scene_desc.m_RenderTargetFormat = graphics::RGBA16_FLOAT;
	m_Scene = new Texture;
	m_Scene->Initiate(scene_desc, "DeferredRenderer - Scene");

	//_______________________________________________________________________

	TextureDesc depth_desc;
	depth_desc.m_Usage = graphics::DEFAULT_USAGE;
	depth_desc.m_ResourceTypeBinding = graphics::BIND_SHADER_RESOURCE | graphics::BIND_DEPTH_STENCIL /*should bind a render target*/;
	depth_desc.m_TextureFormat = graphics::R32_TYPELESS;
	depth_desc.m_DepthTextureFormat = graphics::DEPTH_32_FLOAT;
	depth_desc.m_Width = window_size.m_Width;
	depth_desc.m_Height = window_size.m_Height;

	m_DepthStencilTexture = new Texture;
	m_DepthStencilTexture->Initiate(depth_desc, "DeferredRenderer - DSV");

	//_______________________________________________________________________

	m_ScreenPassShader = Engine::GetInstance()->GetEffect("Shaders/render_to_texture.json");
	m_ScreenPassShader->AddShaderResource(m_Scene, Effect::DIFFUSE);

	m_AmbientPassShader = Engine::GetInstance()->GetEffect("Shaders/deferred_ambient.json");
	//Texture* cubemap = Engine::GetInstance()->GetTexture("Data/Textures/church_horizontal_cross_cube_specular_pow2.dds");
	Texture* cubemap = Engine::GetInstance()->GetTexture("Data/Textures/cube/cubeMapLayered.dds");
	m_AmbientPassShader->AddShaderResource(cubemap, Effect::CUBEMAP);
	//Engine::GetInstance()->GetEffect("Shaders/deferred_pointlight.json");



	//ID3D11Texture2D* tex = static_cast<ID3D11Texture2D*>(texCube->GetTexture());

	//Engine::GetInstance()->GetEffect("Shaders/deferred_spotlight.json")->AddShaderResource(texCube, Effect::CUBEMAP);
	//static_cast<graphics::DirectX11*>(Engine::GetAPI())->SaveTextureToDisk(tex, "cubefile.dds");



	//_______________________________________________________________________

	m_ConstantBuffer = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(m_ConstantStruct), "DeferredRenderer ConstantBuffer");

	//_______________________________________________________________________

	Engine::GetInstance()->GetEffect("Shaders/color_grading.json")->AddShaderResource(m_Scene, Effect::NORMAL);
	m_RenderQuad = new Quad(m_AmbientPassShader);
}

DeferredRenderer::~DeferredRenderer()
{
	SAFE_DELETE(m_RenderQuad);
	SAFE_DELETE(m_Scene);
	SAFE_DELETE(m_DepthStencilTexture);

	Engine::GetAPI()->ReleasePtr(m_ConstantBuffer);
}

void DeferredRenderer::DeferredRender(const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir, const graphics::RenderContext& render_context)
{
	graphics::IGraphicsContext& ctx = render_context.GetContext();
	render_context.GetAPI().ResetViewport();
 
	IDepthStencilView* depth = render_context.GetAPI().GetDepthView();  //What kind of depth????????
	ctx.ClearRenderTarget(m_Scene->GetRenderTargetView(), clearcolor::black);
	ctx.OMSetRenderTargets(1, m_Scene->GetRenderTargetRef(), depth);
	UpdateConstantBuffer(shadow_mvp, light_dir);

	ctx.PSSetConstantBuffer(1, 1, &m_ConstantBuffer);
	ctx.PSSetSamplerState(0, 1, graphics::MSAA_x16);
	ctx.PSSetSamplerState(1, 1, graphics::CUBEMAP);

	ctx.SetRasterizerState(render_context.GetAPI().GetRasterizerState(graphics::CULL_NONE));
	m_RenderQuad->Render();
}
 
 void DeferredRenderer::Finalize() 
 {
	 auto api = Engine::GetAPI();
	 api->GetContext().PSSetSamplerState(0, 1, graphics::MSAA_x1);
	 m_ScreenPassShader->AddShaderResource(m_Scene, Effect::DIFFUSE);
	 m_RenderQuad->Render(false, m_ScreenPassShader);
}

void DeferredRenderer::UpdateConstantBuffer(const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir)
{
	m_ConstantStruct.m_ShadowMVP = shadow_mvp;
	m_ConstantStruct.m_Direction = light_dir;
	auto& ctx = Engine::GetAPI()->GetContext();
	ctx.UpdateConstantBuffer(m_ConstantBuffer, &m_ConstantStruct, sizeof(ConstantStruct));
}

void DeferredRenderer::OnResize()
{
	SAFE_DELETE(m_RenderQuad);
	m_RenderQuad = new Quad(m_AmbientPassShader);
}
