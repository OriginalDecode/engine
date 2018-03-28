#pragma once
#include <Engine/engine_shared.h>
#include <CommonLib/HashString.h>
#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif

class Texture;
namespace graphics
{
	struct CreateInfo
	{
		HWND m_HWND;
		HINSTANCE m_Instance;
		s32 m_WindowWidth = 0;
		s32 m_WindowHeight = 0;
		std::string m_APIName;
	};

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
		NOF_SS
	};

	enum eGraphicsAPI
	{
		NO_API = -1,
		D3D11,
		VULKAN,
	};

	enum eTextureFormat
	{
		NO_FORMAT = BITFLAG(0),
		RGBA32_FLOAT = BITFLAG(1),
		RGBA32_UINT = BITFLAG(2),
		RGBA32_SINT = BITFLAG(3),

		RGB32_FLOAT = BITFLAG(4),
		RGB32_UINT = BITFLAG(5),
		RGB32_SINT = BITFLAG(6),

		RG32_FLOAT = BITFLAG(7),
		RG32_UINT = BITFLAG(8),

		RGBA16_FLOAT = BITFLAG(9),
		RGBA16_UINT = BITFLAG(10),
		RGBA16_SINT = BITFLAG(11),

		RGBA8_UINT = BITFLAG(12),
		RGBA8_SINT = BITFLAG(13),

		R32_TYPELESS = BITFLAG(14),
		R32_FLOAT = BITFLAG(15),
		R32_UINT = BITFLAG(16),
		DEPTH_32_FLOAT = BITFLAG(17),

		RGBA8_UNORM = BITFLAG(18),
		RGB10A2_TYPELESS = BITFLAG(19),
		RGBA8_TYPELESS = BITFLAG(20),
		sRGBA8 = BITFLAG(21),
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

	enum eTopology
	{
		TRIANGLE_LIST,
		POINT_LIST,
		LINE_LIST,
		_4_CONTROL_POINT_PATCHLIST,
	};

	enum eMapping
	{
		MAP_READ,
		MAP_WRITE,
		MAP_READ_WRITE,
		MAP_WRITE_DISCARD,
		MAP_WRITE_NO_OVERWRITE
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

	struct Texture2DDesc
	{
		u32 m_Width = 0;
		u32 m_Height = 0;
		u32 m_MipLevels = 0;
		eTextureFormat m_Format = RGBA32_FLOAT;
		s32 m_Usage = DEFAULT_USAGE;
		s32 m_Binding = NONE;
		s32 m_CPUAccessFlag = 0;
		s32 m_MiscFlags = 0;
		s32 m_ArraySize = 0;
		s32 m_SampleCount = 0;
		s32 m_SampleQuality = 0;
	};

	struct InputElementDesc
	{
		InputElementDesc() = default;

		InputElementDesc(std::string semantic, u32 index, eVertexFormat format, u32 slot, u32 byte_offset, eElementSpecification element_spec, u32 instance_step_rate)
			: m_Semantic(semantic)
			, m_SemanicIndex(index)
			, m_Format(format)
			, m_InputSlot(slot)
			, m_ByteOffset(byte_offset)
			, m_ElementSpecification(element_spec)
			, m_InstanceDataStepRate(instance_step_rate)
		{
		}

		std::string m_Semantic;
		u32 m_SemanicIndex = 0;
		eVertexFormat m_Format;
		u32 m_InputSlot = 0;
		u32 m_ByteOffset = 0;
		eElementSpecification m_ElementSpecification;
		u32 m_InstanceDataStepRate = 0;
	};


	struct SInputLayout
	{

		u32 GetByteSizeOfFormat(eVertexFormat format)
		{
			switch (format)
			{
			case _4BYTE_R_UINT:
			case _4BYTE_R_FLOAT:
				return 4;
			case _8BYTE_RG:
				return 8;
			case _12BYTE_RGB:
				return 12;
			case _16BYTE_RGBA:
				return 16;
			}

			return 0;
		}

		void AddElement(const char* semantic, eVertexFormat format, u32 slot, bool instanced = false, u32 semantic_index = 0)
		{
			eElementSpecification spec = instanced ? INPUT_PER_INSTANCE_DATA : INPUT_PER_VERTEX_DATA;
			u32 instance_step_rate = instanced ? 1 : 0;
			m_Elements.Add(InputElementDesc(semantic, semantic_index, format, slot, m_Offset, spec, instance_step_rate));
			m_Offset += GetByteSizeOfFormat(format);
		}

		void AddElement(const InputElementDesc& obj)
		{
			m_Elements.Add(obj);
		}

		size_t m_Offset = 0;
		CU::GrowingArray<InputElementDesc> m_Elements;

	};

	struct BufferDesc
	{
		eBindFlag m_BindFlag;
		s32 m_CPUAccessFlag;
		eUsage m_UsageFlag;
		s8* m_Data = nullptr;
		s32 m_Size = 0;
		s32 m_StructuredByteStride = 0;
		s32 m_MiscFlags = 0;
		s32 m_ByteWidth = 0;
	};

	class IGraphicsDevice;
	class IGraphicsContext;
	class Viewport;
	class IGraphicsAPI
	{
	public:
		virtual ~IGraphicsAPI() {}
		virtual void Initiate() = 0;

		virtual void EndFrame() = 0;
		virtual void BeginFrame() = 0;


		virtual void OnAltEnter() = 0;
		virtual void OnResize() = 0;


		std::string GetAPIName() { return m_CreateInfo.m_APIName; }
		//virtual void CopyResource(void * pDestination, void * pSource) = 0;

		eGraphicsAPI GetActiveAPI() const { return m_ActiveAPI; }

		virtual Viewport* CreateViewport(u16 width, u16 height, float min_depth, float max_depth, u16 top_left_x, u16 top_left_y) = 0;

		virtual IGraphicsDevice& GetDevice() { return *m_Device; }
		virtual IGraphicsContext& GetContext() { return *m_Context; }

		virtual void ReleasePtr(void* ptr) = 0;

		ISamplerState* GetSamplerState(eSamplerStates sampler_state) { return m_SamplerStates[sampler_state]; }
		IDepthStencilState* GetDepthStencilState(eDepthStencilState depthstencilstate) { return m_DepthStencilStates[depthstencilstate]; }
		IRasterizerState* GetRasterizerState(eRasterizer rasterizer) { return m_RasterizerStates[rasterizer]; }
		IBlendState* GetBlendState(eBlendStates blendstate) { return m_BlendStates[blendstate]; }

		const CreateInfo& GetInfo() const { return m_CreateInfo; }

		virtual void SetDefaultTargets() = 0;

		virtual void ResetViewport() = 0;

		IRenderTargetView* GetBackbuffer() { return m_DefaultRenderTarget; }
		const IRenderTargetView* GetBackbuffer() const { return m_DefaultRenderTarget; }
		IRenderTargetView** GetBackbufferRef() { return &m_DefaultRenderTarget; }

		IDepthStencilView* GetDepthView() { return m_DefaultDepthView; }
		const IDepthStencilView* GetDepthView() const { return m_DefaultDepthView; }


		virtual CU::Vector4f PickColor(Texture* pTexture) = 0;


	protected:
		CreateInfo m_CreateInfo;
		eGraphicsAPI m_ActiveAPI;

		Viewport* m_Viewport						= nullptr;
		IGraphicsDevice* m_Device					= nullptr;
		IGraphicsContext* m_Context					= nullptr;

		ITexture2D* m_DefaultDepthBuffer			= nullptr;
		IDepthStencilView* m_DefaultDepthView		= nullptr;
		IRenderTargetView* m_DefaultRenderTarget	= nullptr;

		ISamplerState* m_SamplerStates[NOF_SS];
		IDepthStencilState* m_DepthStencilStates[NOF_DSS];
		IRasterizerState* m_RasterizerStates[NOF_RS];
		IBlendState* m_BlendStates[NOF_BS];

		


	};
};