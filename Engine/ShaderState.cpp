#include "stdafx.h"
#include "ShaderState.h"
#include <Engine/Engine.h>

ShaderState::ShaderState(const BlendState& blend_state, const SamplerState& sampler_state, const DepthstencilState& depthstencil_state)
	: m_BlendState(blend_state)
	, m_SamplerState(sampler_state)
	, m_DepthstencilState(depthstencil_state)
{
}

void ShaderState::Use(const RenderContext& render_context)
{
	const eGraphicsAPI& api = Engine::GetAPI()->GetActiveAPI();
	if ( api == eGraphicsAPI::D3D11 )
	{
		ID3D11BlendState* blend_state = static_cast<ID3D11BlendState*>(m_BlendState.GetState());
		float clear[4] = { 0.f,0.f,0.f,0.f };
		render_context.m_Context->OMSetBlendState(blend_state, clear, 0xFFFFFFFF);
	}

}



BlendState::BlendState(s32 render_target_write_mask
	, s32 enable_blend_flags
	, BlendOp blend_op, BlendFlag src_blend, BlendFlag dest_blend
	, BlendOp alpha_blend_op, BlendFlag src_blend_alpha, BlendFlag dest_blend_alpha)
{
	m_State = Engine::GetAPI()->CreateBlendState(
		render_target_write_mask,
		enable_blend_flags, 
		blend_op,
		src_blend,
		dest_blend,
		alpha_blend_op,
		src_blend_alpha,
		dest_blend_alpha
	);
}
