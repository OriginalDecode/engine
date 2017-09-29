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

	//_______________________________________________________________________

	TextureDesc scene_desc;
	scene_desc.m_Width = window_size.m_Width;
	scene_desc.m_Height = window_size.m_Height;
	scene_desc.m_Usage = graphics::DEFAULT_USAGE;
	scene_desc.m_ResourceTypeBinding = graphics::BIND_SHADER_RESOURCE | graphics::BIND_RENDER_TARGET;
	scene_desc.m_TextureFormat = graphics::RGBA16_FLOAT;
	scene_desc.m_RenderTargetFormat = graphics::RGBA16_FLOAT;
	m_FinishedSceneTexture = new Texture;
	m_FinishedSceneTexture->Initiate(scene_desc, "DeferredRenderer - Finished Scene");

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
	m_AmbientPassShader = Engine::GetInstance()->GetEffect("Shaders/deferred_ambient.json");
	Texture* cubemap = Engine::GetInstance()->GetTexture("Data/Textures/church_horizontal_cross_cube_specular_pow2.dds");
	m_AmbientPassShader->AddShaderResource(cubemap, Effect::CUBEMAP);

	//_______________________________________________________________________

	m_ConstantBuffer = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(m_ConstantStruct), "DeferredRenderer ConstantBuffer");

	//_______________________________________________________________________

	m_RenderQuad = new Quad(m_AmbientPassShader);
}

DeferredRenderer::~DeferredRenderer()
{
	SAFE_DELETE(m_RenderQuad);
	SAFE_DELETE(m_FinishedSceneTexture);
	SAFE_DELETE(m_DepthStencilTexture);

	Engine::GetAPI()->ReleasePtr(m_ConstantBuffer);
}

void DeferredRenderer::DeferredRender(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& aProjection, const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir, const graphics::RenderContext& render_context)
{

	//why do I reset the viewport???
	//render_context.m_API->ResetViewport();
 
	IRenderTargetView* rtv[] = {
		m_FinishedSceneTexture->GetRenderTargetView()
	};

	//ID3D11DepthStencilView* depth = render_context.m_API->GetDepthView(); 
	//Replace above depth thing
 
	auto& ctx = render_context.GetContext();

	ctx.ClearRenderTarget(rtv, clearcolor::black);
	ctx.OMSetRenderTargets(ARRSIZE(rtv), rtv, nullptr/*depth*/);

	ctx.PSSetConstantBuffer(0, 1, m_ConstantBuffer);
	ctx.PSSetSamplerState(0, 1, render_context.GetAPI().GetSamplerState(graphics::LINEAR_WRAP));

	//render_context.m_API->SetRasterizer(eRasterizer::CULL_NONE);

	//UpdateConstantBuffer(previousOrientation, aProjection, shadow_mvp, light_dir);

	m_RenderQuad->Render(false);

	//Why do I set the finished scene texture as render target? Is this to do post-processing?
	ctx.OMSetRenderTargets(1, &rtv[0], m_DepthStencilTexture->GetDepthView());
}

void DeferredRenderer::SetRenderTarget()
{
// 	ID3D11DepthStencilView* depth = myDepthStencil->GetDepthView();
// 	myContext->OMSetRenderTargets(1, myFinishedSceneTexture->GetRenderTargetRef(), depth);
}
 
 void DeferredRenderer::Finalize() //unsure what this function does for me
 {
// 	m_RenderQuad->Render(false, m_ScreenPassShader);
// 	m_API->SetDepthStencilState(eDepthStencilState::Z_DISABLED, 0);
// 	m_API->SetBlendState(eBlendStates::NO_BLEND);
// 	m_API->SetRasterizer(m_Wireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_NONE);
// 
// 	SetBuffers(render_contexts);
// 
// 	ID3D11ShaderResourceView* srv[] =
// 	{
// 		myFinishedSceneTexture->GetShaderView(),
// 		myDepthStencil->GetShaderView(),
// 	};
// 
// 	s32 num_srv = ARRAYSIZE(srv);
// 
// 	myContext->PSSetShaderResources(0, num_srv, &srv[0]);
// 	m_API->SetSamplerState(eSamplerStates::POINT_CLAMP);
// 
// 	//m_RenderQuad->Render(false);
// 	myScreenPassShader->Use();
// 	myContext->DrawIndexed(6, 0, 0);
// 	myScreenPassShader->Clear();
// 
// 	m_API->SetRasterizer(eRasterizer::CULL_BACK);
// 	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
}

void DeferredRenderer::UpdateConstantBuffer(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir)
{
 	m_ConstantStruct.m_CameraPos = camera_orientation.GetPosition();
 	m_ConstantStruct.m_Projection = CU::Math::InverseReal(camera_projection);
 	m_ConstantStruct.m_View = camera_orientation;
 	m_ConstantStruct.m_ShadowMVP = shadow_mvp;
 	m_ConstantStruct.m_Direction = light_dir;
 
	auto& ctx = Engine::GetAPI()->GetContext();
	ctx.UpdateConstantBuffer(m_ConstantBuffer, &m_ConstantStruct, sizeof(m_ConstantStruct));
}

void DeferredRenderer::OnResize()
{
	SAFE_DELETE(m_RenderQuad);
	m_RenderQuad = new Quad(m_AmbientPassShader);
}
