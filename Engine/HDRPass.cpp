#include "stdafx.h"
#include "HDRPass.h"
#include <Utilities.h>
#include "Window.h"
#include "Engine.h"
#include "DirectX11.h"
#include "Synchronizer.h"
#include "RenderCommand.h"

#include <profile_defines.h>
#include <Engine/Viewport.h>
#include <Engine/Quad.h>

void HDRPass::Initiate()
{
	WindowSize window_size = Engine::GetInstance()->GetInnerSize();

	s32 pow2 = cl::nearest_Pow_Under(window_size.m_Height);

	m_DefaultViewport = Engine::GetAPI()->CreateViewport(pow2, pow2, 0.f, 1.f, 0, 0);
	m_ChangeableViewport = Engine::GetAPI()->CreateViewport(pow2, pow2, 0.f, 1.f, 0, 0);

	TextureDesc desc;
	
	desc.m_ResourceTypeBinding = graphics::BIND_RENDER_TARGET | graphics::BIND_SHADER_RESOURCE;
	desc.m_Width = window_size.m_Width;
	desc.m_Height = window_size.m_Height;
	desc.m_TextureFormat = graphics::RGBA8_UNORM;
	desc.m_ShaderResourceFormat = graphics::RGBA8_UNORM;
	desc.m_RenderTargetFormat = graphics::RGBA8_UNORM;

	m_HDRTexture = new Texture;
	m_HDRTexture->Initiate(desc,  "HDRPass | HDRTexture");

	s32 downsample_amount = s32(log(__min(window_size.m_Width, window_size.m_Height)) / log(2.f)) + 1; //can be changed
	s32 sample_size = 1;
	desc.m_ShaderResourceFormat = graphics::RGBA16_FLOAT;
	desc.m_RenderTargetFormat = graphics::RGBA16_FLOAT;
	for (s32 i = 0; i < downsample_amount; i++)
	{
		std::stringstream debug_name;
		debug_name << "HDRPass Downsample : "<< sample_size << "x" << sample_size;
		m_Downsamples.Add(new Texture);
		
		desc.m_Width = sample_size;
		desc.m_Height = sample_size;

		m_Downsamples.GetLast()->Initiate(desc,	false, debug_name.str().c_str());

		sample_size *= 2.f;
	}

	m_HDREffect = Engine::GetInstance()->GetEffect("Shaders/tonemapping.json");
	m_DownsampleEffect = Engine::GetInstance()->GetEffect("Shaders/downsample_hdr.json");
	m_RenderToScreenEffect = Engine::GetInstance()->GetEffect("Shaders/render_to_texture.json");

	m_Quad = new Quad;
}

void HDRPass::CleanUp()
{
	SAFE_DELETE(m_HDRTexture);
	for (Texture* texture : m_Downsamples)
	{
		delete texture;
		texture = nullptr;
	}
	delete m_Quad;
	m_Quad = nullptr;
}

void HDRPass::Process(Texture* scene_texture, const graphics::RenderContext& render_context)
{
	PROFILE_FUNCTION(profiler::colors::Blue);
	//set buffers

	graphics::IGraphicsContext& ctx = render_context.GetContext();
	ctx.SetViewport(m_DefaultViewport);
	
	const s32 downsamples = m_Downsamples.Size() - 1;
	Downsample(m_Downsamples[downsamples]->GetRenderTargetView(), scene_texture->GetShaderView());

	for (s32 i = downsamples - 1; i >= 0; --i)
	{
		m_ChangeableViewport->SetHeight(m_Downsamples[i]->GetHeight());
		m_ChangeableViewport->SetWidth(m_Downsamples[i]->GetWidth());
		ctx.SetViewport(m_ChangeableViewport);

		Downsample(m_Downsamples[i]->GetRenderTargetView(), m_Downsamples[i + 1]->GetShaderView());

	}

	//m_API->ResetViewport();// this goes back to the **DEFAULT DEFAULT ONE**

	IShaderResourceView* sources[] = {
		scene_texture->GetShaderView(),
		m_Downsamples.GetLast()->GetShaderView(),
		m_Downsamples[0]->GetShaderView() // average
	};

	Tonemapping(m_HDRTexture->GetRenderTargetView(), sources, ARRAYSIZE(sources));

	//m_API->ResetRendertarget();
	ctx.PSSetShaderResource(0, 1, m_HDRTexture->GetShaderViewRef());
	ctx.DrawIndexed(m_Quad, m_RenderToScreenEffect);
}

void HDRPass::Downsample(IRenderTargetView* render_target, IShaderResourceView* source)
{
	graphics::IGraphicsContext& ctx = Engine::GetAPI()->GetContext();
	ctx.OMSetRenderTargets(1, &render_target, nullptr);
	ctx.ClearRenderTarget(render_target, clearcolor::black);
	ctx.PSSetShaderResource(0, 1, &source);
	ctx.DrawIndexed(m_Quad, m_DownsampleEffect);
}

void HDRPass::Tonemapping(IRenderTargetView* target, IShaderResourceView* source[2], s32 resource_count)
{
	graphics::IGraphicsContext& ctx = Engine::GetAPI()->GetContext();
	ctx.ClearRenderTarget(target, clearcolor::black);
	ctx.OMSetRenderTargets(1, &target, nullptr);
	
	ctx.PSSetShaderResource(0, resource_count, source);
	ctx.DrawIndexed(m_Quad, m_HDREffect);
}

void HDRPass::OnResize()
{
}