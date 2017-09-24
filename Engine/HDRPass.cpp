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
	desc.m_ShaderResourceFormat = graphics::RGBA8_UNORM;
	desc.m_RenderTargetFormat = graphics::RGBA8_UNORM;

	m_HDRTexture = new Texture;
	m_HDRTexture->Initiate(desc, "HDRPass | HDRTexture");

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

		m_Downsamples.GetLast()->Initiate(desc,	debug_name.str().c_str());

		sample_size *= 2.f;
	}

	m_HDREffect = Engine::GetInstance()->GetEffect("Shaders/tonemapping.json");
	m_DownsampleEffect = Engine::GetInstance()->GetEffect("Shaders/downsample_hdr.json");
	m_RenderToScreenEffect = Engine::GetInstance()->GetEffect("Shaders/render_to_texture.json");
}

void HDRPass::CleanUp()
{
	SAFE_DELETE(m_HDRTexture);

	for (Texture* texture : m_Downsamples)
	{
		delete texture;
		texture = nullptr;
	}
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

	m_API->ResetViewport();// this goes back to the **DEFAULT DEFAULT ONE**

	IShaderResourceView* sources[] = {
		scene_texture->GetShaderView(),
		m_Downsamples.GetLast()->GetShaderView(),
		m_Downsamples[0]->GetShaderView() // average
	};

	Tonemapping(m_HDRTexture->GetRenderTargetView(), sources, ARRAYSIZE(sources));

	m_API->ResetRendertarget();
	m_RenderToScreenEffect->Use();
	IDevContext* ctx = m_API->GetContext();
	ctx->PSSetShaderResources(0, 1, m_HDRTexture->GetShaderViewRef());
	//no buffers has been set??????????????????????????????
	ctx->DrawIndexed(6, 0, 0);

	m_RenderToScreenEffect->Clear();

}

void HDRPass::Downsample(IRenderTargetView* render_target, IShaderResourceView* source)
{
	IDevContext* ctx = m_API->GetContext();
	ctx->OMSetRenderTargets(1, &render_target, nullptr);

	const float clear[4] = { 0,0,0,0 };
	ctx->ClearRenderTargetView(render_target, clear);

	m_DownsampleEffect->Use();
	ctx->PSSetShaderResources(0, 1, &source);
	ctx->DrawIndexed(6, 0, 0);

	m_DownsampleEffect->Clear();
}

void HDRPass::Tonemapping(IRenderTargetView* target, IShaderResourceView* source[2], s32 resource_count)
{
	IDevContext* ctx = m_API->GetContext();
	const float clear[4] = { 0,0,0,0 };
	ctx->ClearRenderTargetView(target, clear);
	ctx->OMSetRenderTargets(1, &target, m_API->GetDepthView());
	
	m_HDREffect->Use();
	ctx->PSSetShaderResources(0, resource_count, source);

	ctx->DrawIndexed(6, 0, 0);
	m_HDREffect->Clear();
}

void HDRPass::OnResize()
{
}