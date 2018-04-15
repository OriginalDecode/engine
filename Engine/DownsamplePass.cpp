#include "stdafx.h"
//#include "DownsamplePass.h"
//
//
//
//DownsamplePass::DownsamplePass(s32 sample_count, s32 width, s32 height)
//	: m_Downsampler(Engine::GetInstance()->GetEffect("Shaders/downsample_hdr.json"))
//{
//
//}
//
//DownsamplePass::~DownsamplePass()
//{
//	//Engine::GetAPI()->ReleasePtr(m_Buffer);
//	//delete m_Reflection;
//	//m_Reflection = nullptr;
//	delete m_Quad;
//	m_Quad = nullptr;
//}
//
//void DownsamplePass::Process(Texture* scene, const graphics::RenderContext& rc)
//{
//
//}
//
//void DownsamplePass::Process(s32 sample_count, Texture* pTexture, const graphics::RenderContext& rc)
//{
//
//	for (s32 i = 0; i < sample_count; ++i)
//	{
//		Process()
//	}
//
//}
//
//// DOWNSAMPLE START
//
//const s32 downsamples = m_Downsamples.Size() - 1;
//IBlendState* pBlend = render_context.GetAPI().GetBlendState(graphics::NO_BLEND);
//ctx.SetBlendState(pBlend);
//Downsample(m_Downsamples[downsamples]->GetRenderTargetView(), scene_texture->GetShaderView());
//
//
//for (s32 i = downsamples - 1; i >= 0; --i)
//{
//	m_ChangeableViewport->SetHeight(m_Downsamples[i]->GetHeight());
//	m_ChangeableViewport->SetWidth(m_Downsamples[i]->GetWidth());
//	ctx.SetViewport(m_ChangeableViewport);
//
//	ctx.SetBlendState(pBlend);
//	Downsample(m_Downsamples[i]->GetRenderTargetView(), m_Downsamples[i + 1]->GetShaderView());
//
//}
//
//render_context.GetAPI().ResetViewport();
//
//// DOWNSAMPLE END
//
//
//void HDRPass::Downsample(IRenderTargetView* render_target, IShaderResourceView* source)
//{
//	graphics::IGraphicsContext& ctx = Engine::GetAPI()->GetContext();
//	ctx.OMSetRenderTargets(1, &render_target, nullptr);
//	ctx.ClearRenderTarget(render_target, clearcolor::black);
//	m_DownsampleEffect->AddShaderResource(source, Effect::DIFFUSE);
//	m_Quad->Render(false, m_DownsampleEffect);
//}
//
//void DownsamplePass::OnResize()
//{
//
//}
