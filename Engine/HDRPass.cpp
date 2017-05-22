#include "stdafx.h"
#include "HDRPass.h"
#include <Utilities.h>
#include "Window.h"
#include "Engine.h"
#include "DirectX11.h"
#include "Synchronizer.h"
#include "RenderCommand.h"

void HDRPass::Initiate()
{
	m_Engine = Engine::GetInstance();
	m_WindowSize = m_Engine->GetInnerSize();
	m_API = m_Engine->GetAPI();


	m_HDRTexture = new Texture;
	m_HDRTexture->Initiate(
		m_WindowSize.m_Width,
		m_WindowSize.m_Height,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 
		DXGI_FORMAT_R8G8B8A8_UNORM, 
		"HDRPass | HDRTexture");
	s32 downsample_amount = s32(log(__min(m_WindowSize.m_Width, m_WindowSize.m_Height)) / log(2.f)) + 1;
	s32 sample_size = 1;
	for (s32 i = 0; i < downsample_amount; i++)
	{
		std::stringstream debug_name;
		debug_name << "HDRPass Downsample : "<< sample_size << "x" << sample_size;
		m_Downsamples.Add(new Texture);
		m_Downsamples.GetLast()->Initiate(
			sample_size,
			sample_size,
			D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 
			DXGI_FORMAT_R16G16B16A16_FLOAT, 
			debug_name.str());

		sample_size *= 2.f;
	}

	m_HDREffect = m_Engine->GetEffect("Shaders/T_Tonemapping.json");
	m_DownsampleEffect = m_Engine->GetEffect("Shaders/T_Tonemapping_Downsample.json");
	m_RenderToScreenEffect = m_Engine->GetEffect("Shaders/T_Render_To_Texture.json");
}

void HDRPass::CleanUp()
{
	m_HDRTexture->CleanUp();
	SAFE_DELETE(m_HDRTexture);

	for (Texture* texture : m_Downsamples)
	{
		texture->CleanUp();
		delete texture;
		texture = nullptr;
	}
}

void HDRPass::Process(Texture* scene_texture)
{
	//set buffers

	Viewport viewport;
	FillViewportData(viewport);
	m_API->SetViewport(&viewport);

	y_height = 64.f;
	x_width = 364.f;
	const s32 downsamples = m_Downsamples.Size() - 1;
	Downsample(m_Downsamples[downsamples]->GetRenderTargetView(), scene_texture->GetShaderView());

	for (s32 i = downsamples - 1; i >= 0; --i)
	{
		viewport.Height = (float)m_Downsamples[i]->GetHeight();
		viewport.Width = (float)m_Downsamples[i]->GetWidth();

		m_API->SetViewport(&viewport);

		Downsample(m_Downsamples[i]->GetRenderTargetView(), m_Downsamples[i + 1]->GetShaderView());

	}

	m_API->ResetViewport();

	IShaderResourceView* sources[] = {
		scene_texture->GetShaderView(),
		m_Downsamples.GetLast()->GetShaderView(),
		m_Downsamples[0]->GetShaderView() // average
	};

	Tonemapping(m_HDRTexture->GetRenderTargetView(), sources, ARRAYSIZE(sources));

	m_API->ResetRendertarget();

	m_RenderToScreenEffect->Activate();
	IDevContext* ctx = m_API->GetContext();
	ctx->PSSetShaderResources(0, 1, m_HDRTexture->GetShaderViewRef());
	ctx->DrawIndexed(6, 0, 0);

	m_RenderToScreenEffect->Deactivate();

}

void HDRPass::Downsample(IRenderTargetView* render_target, IShaderResourceView* source)
{
	IDevContext* ctx = m_API->GetContext();
	ctx->OMSetRenderTargets(1, &render_target, nullptr);

	const float clear[4] = { 0,0,0,0 };
	ctx->ClearRenderTargetView(render_target, clear);

	m_DownsampleEffect->Activate();
	ctx->PSSetShaderResources(0, 1, &source);
	ctx->DrawIndexed(6, 0, 0);

	m_DownsampleEffect->Deactivate();

#ifdef _DEBUG
	if (toggle_debug)
	{
		//m_Engine->GetSynchronizer()->AddRenderCommand(RenderCommand(eType::SPRITE, source, CU::Vector2f(x_width, y_height)));
	}
	y_height += 128.f;
	if (y_height > 1080.f)
	{
		x_width += 128.f;
		y_height = 64.f;
	}
#endif
}

void HDRPass::Tonemapping(IRenderTargetView* target, IShaderResourceView* source[2], s32 resource_count)
{
	IDevContext* ctx = m_API->GetContext();
	const float clear[4] = { 0,0,0,0 };
	ctx->ClearRenderTargetView(target, clear);
	ctx->OMSetRenderTargets(1, &target, m_API->GetDepthView());
	
	m_HDREffect->Activate();
	ctx->PSSetShaderResources(0, resource_count, source);

	ctx->DrawIndexed(6, 0, 0);
	m_HDREffect->Deactivate();

}

void HDRPass::FillViewportData(Viewport& viewport)
{
	s32 pow2 = CL::nearest_Pow_Under(m_WindowSize.m_Height);

	//s32 width_diff = m_WindowSize.m_Width - pow2;
	//s32 height_diff = m_WindowSize.m_Height - pow2;


	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.Width = pow2;
	viewport.Height = pow2;
}

void HDRPass::OnResize()
{

}