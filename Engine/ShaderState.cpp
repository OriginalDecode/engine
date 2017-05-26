#include "stdafx.h"
#include "ShaderState.h"
#include <Engine/Engine.h>

ShaderState::ShaderState(const BlendState& blend_state, const SamplerState& sampler_state, const DepthstencilState& depthstencil_state)
	: m_BlendState(blend_state)
	, m_SamplerState(sampler_state)
	, m_DepthstencilState(depthstencil_state)
{
}

void ShaderState::Use()
{
}
