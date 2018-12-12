#include "stdafx.h"
#include "DownsamplePass.h"
#include <Engine/Viewport.h>
#include <Engine/Quad.h>
#include <Engine/DebugHandle.h>

#include <Engine/profile_defines.h>

DownsamplePass::~DownsamplePass()
{
	delete m_Quad;
	m_Quad = nullptr;

	delete m_Viewport;
	m_Viewport = nullptr;

	m_Downsamples.DeleteAll();

}

void DownsamplePass::Initiate(int32 sample_count, int32 width, int32 height, TextureDesc& desc, bool register_textures /* = false */)
{

	m_Downsampler = Engine::GetInstance()->GetEffect("Shaders/downsample_hdr.json");
	m_Height = height;
	m_Width = width;

	m_Viewport = Engine::GetAPI()->CreateViewport(width, height, 0, 1, 0, 0);

	int32 tex_height = m_Height;
	int32 tex_width = m_Width;

	int32 sample_size = 1;

	for (int32 i = 0; i < sample_count; ++i)
	{
		desc.m_Width = sample_size;
		desc.m_Height = sample_size;
		m_Downsamples.Add(new Texture);

		std::stringstream debug_name;
#ifdef _DEBUG
		debug_name << "Downsample : " << sample_size << "x" << sample_size;
		if (register_textures)
		{
			debug::DebugHandle* pDebug = debug::DebugHandle::GetInstance();
			pDebug->RegisterTexture(m_Downsamples.GetLast(), debug_name.str().c_str());
		}
#endif

		m_Downsamples.GetLast()->Initiate(desc, false, debug_name.str());

		sample_size *= 2;
	}

	m_Quad = new Quad;
}

void DownsamplePass::Process(Texture* scene, const graphics::RenderContext& rc)
{
	PROFILE_FUNCTION(profiler::colors::Blue);

	const int32 downsamples = m_Downsamples.Size() - 1;

	graphics::IGraphicsContext& ctx = rc.GetContext();
	ctx.SetBlendState(graphics::NO_BLEND);


	m_Viewport->SetHeight(m_Height);
	m_Viewport->SetWidth(m_Width);

	Downsample(m_Downsamples[downsamples]->GetRenderTargetView(), scene->GetShaderView());

	for (int32 i = downsamples - 1; i >= 0; --i)
	{
		m_Viewport->SetHeight(m_Downsamples[i]->GetHeight());
		m_Viewport->SetWidth(m_Downsamples[i]->GetWidth());
		ctx.SetViewport(m_Viewport);

		ctx.SetBlendState(graphics::NO_BLEND);
		Downsample(m_Downsamples[i]->GetRenderTargetView(), m_Downsamples[i + 1]->GetShaderView());

	}

	rc.GetAPI().ResetViewport();
}
void DownsamplePass::Downsample(IRenderTargetView* render_target, IShaderResourceView* source)
{
	graphics::IGraphicsContext& ctx = Engine::GetAPI()->GetContext();
	ctx.OMSetRenderTargets(1, &render_target, nullptr);
	ctx.ClearRenderTarget(render_target, clearcolor::black);
	m_Downsampler->AddShaderResource(source, TextureSlot::DIFFUSE);
	m_Quad->Render(false, m_Downsampler);
}

void DownsamplePass::OnResize()
{
	assert(false && "not implemented");
}

Texture* DownsamplePass::GetSample(int32 index) const
{
	return m_Downsamples[index];
}
