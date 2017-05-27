#pragma once
#include <Engine/snowblind_shared.h>

struct BlendState
{
public:
	enum EnableBlendFlags
	{
		BLEND_ENABLED = BITFLAG(0),
		ALPHA_TO_COVERAGE = BITFLAG(1),
		INDEPENDANT_BLEND_ENABLED = BITFLAG(2),
	};

	enum BlendFlag
	{
		BLEND_ZERO = 1,
		BLEND_ONE = 2,
		BLEND_SRC_COLOR = 3,
		BLEND_INV_SRC_COLOR = 4,
		BLEND_SRC_ALPHA = 5,
		BLEND_INV_SRC_ALPHA = 6,
		BLEND_DEST_ALPHA = 7,
		BLEND_INV_DEST_ALPHA = 8,
		BLEND_DEST_COLOR = 9,
		BLEND_INV_DEST_COLOR = 10,
		BLEND_SRC_ALPHA_SAT = 11,
		BLEND_BLEND_FACTOR = 14,
		BLEND_INV_BLEND_FACTOR = 15,
		BLEND_SRC1_COLOR = 16,
		BLEND_INV_SRC1_COLOR = 17,
		BLEND_SRC1_ALPHA = 18,
		BLEND_INV_SRC1_ALPHA = 19
	};

	enum BlendOp
	{
		BLEND_OP_ADD = 1,
		BLEND_OP_SUBTRACT = 2,
		BLEND_OP_REV_SUBTRACT = 3,
		BLEND_OP_MIN = 4,
		BLEND_OP_MAX = 5
	};



	BlendState(s32 render_target_write_mask, s32 enable_blend_flags, BlendOp blend_op,  BlendFlag src_blend, BlendFlag dest_blend, BlendOp alpha_blend_op, BlendFlag src_blend_alpha, BlendFlag dest_blend_alpha);
	BlendState() = default;

	void* GetState() { return m_State; }
private:
	void* m_State = nullptr;
	s32 m_BlendFlags = 0;
	s32 m_BlendOps = 0;
	s32 m_RenderTargetWriteMask = 0;

};

struct SamplerState
{
public:

private:
	void* m_State = nullptr;
};

struct DepthstencilState
{
public:

private:
	void* m_State = nullptr;
};

struct RasterizerState
{
public:

private:
	void* m_State = nullptr;
};


class ShaderState
{
public:
	ShaderState(const BlendState& blend_state, const SamplerState& sampler_state, const DepthstencilState& depthstencil_state);
	ShaderState() = default;
	void Use(const RenderContext& render_context);




private:

	BlendState			m_BlendState;
	SamplerState		m_SamplerState;
	DepthstencilState	m_DepthstencilState;


};

