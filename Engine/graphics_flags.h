#pragma once

namespace graphics
{

	enum eDepthStencilState
	{
		Z_ENABLED,
		Z_DISABLED,
		READ_NO_WRITE,
		READ_NO_WRITE_PARTICLE,
		NOF_DSS
	};

	enum eRasterizer
	{
		WIREFRAME,
		CULL_BACK,
		CULL_NONE,
		CULL_FRONT,
		NOF_RS
	};

	enum eBlendStates
	{
		NO_BLEND,
		LIGHT_BLEND,
		ALPHA_BLEND,
		BLEND_FALSE,
		PARTICLE_BLEND,
		NOF_BS
	};

	enum eSamplerStates
	{
		MSAA_x1,
		MSAA_x2,
		MSAA_x4,
		MSAA_x8,
		MSAA_x16,
		TRILINEAR,
		BILINEAR,
		LINEAR_WRAP,
		LINEAR_CLAMP,
		CUBEMAP,
		POINT_CLAMP,
		POINT_WRAP,
		NOF_SS
	};

	struct SamplerState
	{


		enum ShaderBinding
		{
			VERTEX_SHADER,
			FRAGMENT_SHADER,
			GEOMETRY_SHADER,
			HULL_SHADER,
			DOMAIN_SHADER,
			COMPUTE_SHADER,
			NOF_TYPES
		};

		SamplerState() = default;
		SamplerState(eSamplerStates sampler_state, ShaderBinding binding)
			: m_SamplerState(sampler_state)
			, m_ShaderBinding(binding)
		{
		}

		ShaderBinding m_ShaderBinding;
		eSamplerStates m_SamplerState;

	};


	enum eTopology
	{
		TRIANGLE_LIST,
		POINT_LIST,
		LINE_LIST,
		_4_CONTROL_POINT_PATCHLIST,
	};

	enum eGraphicsAPI
	{
		NO_API = -1,
		D3D11,
		D3D12,
		VULKAN,
	};

	enum eMapping
	{
		MAP_READ,
		MAP_WRITE,
		MAP_READ_WRITE,
		MAP_WRITE_DISCARD,
		MAP_WRITE_NO_OVERWRITE
	};

	enum eTextureFormat
	{
		NO_FORMAT = 1 << 0,
		RGBA32_FLOAT = 1 << 1,
		RGBA32_UINT = 1 << 2,
		RGBA32_SINT = 1 << 3,

		RGB32_FLOAT = 1 << 4,
		RGB32_UINT = 1 << 5,
		RGB32_SINT = 1 << 6,

		RG32_FLOAT = 1 << 7,
		RG32_UINT = 1 << 8,

		RGBA16_FLOAT = 1 << 9,
		RGBA16_UINT = 1 << 10,
		RGBA16_SINT = 1 << 11,

		RGBA8_UINT = 1 << 12,
		RGBA8_SINT = 1 << 13,

		R32_TYPELESS = 1 << 14,
		R32_FLOAT = 1 << 15,
		R32_UINT = 1 << 16,
		DEPTH_32_FLOAT = 1 << 17,

		RGBA8_UNORM = 1 << 18,
		RGB10A2_TYPELESS = 1 << 19,
		RGBA8_TYPELESS = 1 << 20,
		sRGBA8 = 1 << 21,
	};

	enum eVertexFormat
	{
		_4BYTE_R_FLOAT,
		_4BYTE_R_UINT,
		_8BYTE_RG,
		_12BYTE_RGB,
		_16BYTE_RGBA,
	};

	enum eUsage
	{
		DEFAULT_USAGE = 0,
		IMMUTABLE_USAGE = 1,
		DYNAMIC_USAGE = 2,
		STAGING_USAGE = 4,
	};

	enum eCPUAccessFlag
	{
		NO_ACCESS_FLAG = 0,
		READ = 1,
		WRITE = 2,
	};

	enum eElementSpecification
	{
		INPUT_PER_VERTEX_DATA,
		INPUT_PER_INSTANCE_DATA,
	};

	enum eBindFlag
	{
		NONE,
		BIND_VERTEX_BUFFER = 1,
		BIND_INDEX_BUFFER = 2,
		BIND_CONSTANT_BUFFER = 4,
		BIND_SHADER_RESOURCE = 8,
		BIND_STREAM_OUTPUT = 16,
		BIND_RENDER_TARGET = 32,
		BIND_DEPTH_STENCIL = 64,
		BIND_UNORDERED_ACCESS = 128,
		BIND_DECODER = 256,
		BIND_VIDEO_ENCODER = 512
	};

	enum eClearFlag
	{
		DEPTH,
		STENCIL
	};
};