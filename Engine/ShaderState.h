#pragma once
#include <Engine/engine_shared.h>

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
	void* GetState() { return m_State; }



	enum ShaderType
	{
		VERTEX_SHADER,
		PIXEL_SHADER,
		GEOMETRY_SHADER,
		DOMAIN_SHADER,
		HULL_SHADER,
		COMPUTE_SHADER,
		NOF_TYPES
	};

	enum UVAddressMode
	{
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MIRROR_ONCE,
	};

	enum ComparisonFunc
	{
		COMPARISON_NEVER = 1,
		COMPARISON_LESS = 2,
		COMPARISON_EQUAL = 3,
		COMPARISON_LESS_EQUAL = 4,
		COMPARISON_GREATER = 5,
		COMPARISON_NOT_EQUAL = 6,
		COMPARISON_GREATER_EQUAL = 7,
		COMPARISON_ALWAYS = 8
	};

	enum FilterMode
	{
		FILTER_MIN_MAG_MIP_POINT = 0,
		FILTER_MIN_MAG_POINT_MIP_LINEAR = 0x1,
		FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
		FILTER_MIN_POINT_MAG_MIP_LINEAR = 0x5,
		FILTER_MIN_LINEAR_MAG_MIP_POINT = 0x10,
		FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
		FILTER_MIN_MAG_LINEAR_MIP_POINT = 0x14,
		FILTER_MIN_MAG_MIP_LINEAR = 0x15,
		FILTER_ANISOTROPIC = 0x55,
		FILTER_COMPARISON_MIN_MAG_MIP_POINT = 0x80,
		FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
		FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
		FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
		FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
		FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
		FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
		FILTER_COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
		FILTER_COMPARISON_ANISOTROPIC = 0xd5,
		FILTER_MINIMUM_MIN_MAG_MIP_POINT = 0x100,
		FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x101,
		FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x104,
		FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x105,
		FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x110,
		FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x111,
		FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x114,
		FILTER_MINIMUM_MIN_MAG_MIP_LINEAR = 0x115,
		FILTER_MINIMUM_ANISOTROPIC = 0x155,
		FILTER_MAXIMUM_MIN_MAG_MIP_POINT = 0x180,
		FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x181,
		FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x184,
		FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x185,
		FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x190,
		FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x191,
		FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x194,
		FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR = 0x195,
		FILTER_MAXIMUM_ANISOTROPIC = 0x1d5
	};

	SamplerState() = default;
	SamplerState(s32 slot, FilterMode filter, ShaderType shader_type, UVAddressMode address_mode, u32 max_anisotropy, float mip_lod_bias, float min_lod, float max_lod, float* border_color, ComparisonFunc comparison_function);
	const ShaderType& GetShaderType() { return m_ShaderType; }
	const ShaderType& GetShaderType() const  { return m_ShaderType; }
	s32 GetSlot() const { return m_Slot; }
	u32 GetMaxAnisotropy() const { return m_MaxAnisotropy; }
	float GetMinLOD() const { return m_MinLOD; }
	float GetMaxLOD() const { return m_MaxLOD; }
	float GetLODBiat() const { return m_MipLODBias; }
	const UVAddressMode& GetUVAddressMode() const { return m_AddressMode; }
	const ComparisonFunc& GetComparisonFunc() const { return m_ComparisonFunc; }
	const FilterMode& GetFilterMode() const { return m_FilterMode; }

private:
	void* m_State = nullptr;
	ShaderType m_ShaderType;
	s32 m_Slot = 0;
	UVAddressMode m_AddressMode;
	u32 m_MaxAnisotropy;
	float m_MinLOD;
	float m_MaxLOD;
	float m_MipLODBias;
	float m_BorderColor[4];
	ComparisonFunc m_ComparisonFunc;
	FilterMode m_FilterMode;


};

struct DepthstencilState
{
public:
	void* GetState() { return m_State; }
private:
	void* m_State = nullptr;
};

struct RasterizerState
{
public:
	void* GetState() { return m_State; }
private:
	void* m_State = nullptr;
};


class ShaderState
{
public:
	ShaderState() = default;
	ShaderState(const BlendState& blend_state, SamplerState sampler_state[], u32 sampler_state_count, const DepthstencilState& depthstencil_state, const RasterizerState& rasterizer_state);
	void Use(const RenderContext& render_context);


	BlendState& GetBlendState();
	CU::GrowingArray<SamplerState>& GetSamplerState(s32 sampler_index);
	DepthstencilState& GetDepthstencilState();
	RasterizerState& GetRasterizerState();

private:

	BlendState			m_BlendState;
	CU::GrowingArray<SamplerState> m_SamplerStates[SamplerState::NOF_TYPES];
	DepthstencilState	m_DepthstencilState;
	RasterizerState		m_RasterizerState;

};

