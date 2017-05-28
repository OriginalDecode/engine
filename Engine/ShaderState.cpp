#include "stdafx.h"
#include "ShaderState.h"
#include <Engine/Engine.h>

ShaderState::ShaderState(const BlendState& blend_state
	, SamplerState sampler_state[]
	, u32 sampler_state_count
	, const DepthstencilState& depthstencil_state
	, const RasterizerState& rasterizer_state
) // break
	: m_BlendState(blend_state)
	, m_DepthstencilState(depthstencil_state)
	, m_RasterizerState(rasterizer_state)
{
	for (s32 i = 0; i < sampler_state_count; i++)
	{
		SamplerState state = sampler_state[i];
		m_SamplerStates[state.GetShaderType()].Add(state);
	}
}

void ShaderState::Use(const RenderContext& render_context)
{
	render_context.m_API->SetShaderState(*this);
}

BlendState& ShaderState::GetBlendState()
{
	return m_BlendState;
}

CU::GrowingArray<SamplerState>& ShaderState::GetSamplerState(s32 sampler_index)
{
	return m_SamplerStates[sampler_index];
}

DepthstencilState& ShaderState::GetDepthstencilState()
{
	return m_DepthstencilState;
}

RasterizerState& ShaderState::GetRasterizerState()
{
	return m_RasterizerState;
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

SamplerState::SamplerState(s32 slot, FilterMode filter, ShaderType shader_type, UVAddressMode address_mode, u32 max_anisotropy, float mip_lod_bias, float min_lod, float max_lod, float* border_color, ComparisonFunc comparison_function)
	: m_Slot(slot)
	, m_FilterMode(filter)
	, m_ShaderType(shader_type)
	, m_AddressMode(address_mode)
	, m_MaxAnisotropy(max_anisotropy)
	, m_MaxLOD(max_lod)
	, m_MinLOD(min_lod)
	, m_MipLODBias(mip_lod_bias)
{
	memcpy(&m_BorderColor[0], m_BorderColor, sizeof(float) * 4);

	m_State = Engine::GetAPI()->CreateSamplerState(
		m_FilterMode,
		m_AddressMode,
		m_MaxAnisotropy,
		m_MipLODBias,
		m_MinLOD,
		m_MaxLOD,
		m_BorderColor,
		m_ComparisonFunc
	);
}
