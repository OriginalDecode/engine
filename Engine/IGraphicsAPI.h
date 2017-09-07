#pragma once
#include <Engine/engine_shared.h>
#include <Engine/ShaderState.h>
#include <CommonLib/HashString.h>
#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif
#include <Engine/IGraphicsContext.h>
#include <Engine/IGraphicsDevice.h>

namespace graphics
{
	struct CreateInfo
	{
		HWND m_HWND;
		HINSTANCE m_Instance;
		float m_WindowWidth = 0.f;
		float m_WindowHeight = 0.f;
		std::string m_APIName;
	};

	enum eDepthStencilState
	{
		Z_ENABLED,
		Z_DISABLED,
		READ_NO_WRITE,
		READ_NO_WRITE_PARTICLE,
		MASK_TEST,
		LIGHT_MASK,
		DEPTH_TEST,
		NOF_DSS
	};

	enum eRasterizer
	{
		WIREFRAME,
		CULL_BACK,
		CULL_NONE,
		CULL_FRONT,
		MSAA,
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
		LINEAR_CLAMP,
		LINEAR_WRAP,
		POINT_CLAMP,
		POINT_WRAP,
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
		RGBA32_FLOAT,
		RGBA32_UINT,
		RGBA32_SINT,

		RGB32_FLOAT,
		RGB32_UINT,
		RGB32_SINT,

		RGBA16_FLOAT,
		RGBA16_UINT,
		RGBA16_SINT,

		RGB16_FLOAT,
		RGB16_UINT,
		RGB16_SINT,


		RGBA8_FLOAT,
		RGBA8_UINT,
		RGBA8_SINT,

		RGB8_FLOAT,
		RGB8_UINT,
		RGB8_SINT,

		R32_TYPELESS, 
		R32_FLOAT,
		DEPTH_32_FLOAT, 


	};

	enum eTextureUsage
	{
		DEFAULT_USAGE = 0,
		IMMUTABLE_USAGE = 1,
		DYNAMIC_USAGE = 2,
		STAGING_USAGE = 4,
	};

	enum eTextureBind
	{
		RENDERTARGET = 0,
		DEPTHSTENCIL = 1,
		SHADERRESOURCE = 2,
	};

	enum eCPUAccessFlag
	{
		NO_ACCESS_FLAG = 0,
		READ = 1,
		WRITE = 2,
	};

	struct Texture2DDesc
	{
		u32 m_Width = 0;
		u32 m_Height = 0;
		u32 m_MipLevels = 0;
		eTextureFormat m_Format = RGBA32_FLOAT;
		s32 m_Usage = DEFAULT_USAGE;
		s32 m_Binding = SHADERRESOURCE;
		s32 m_CPUAccessFlag = 0;
		s32 m_MiscFlags = 0;
		s32 m_ArraySize = 0;
		s32 m_SampleCount = 0;
		s32 m_SampleQuality = 0;
	};

	class IGraphicsAPI
	{
		friend class IGraphicsDevice;
		friend class IGraphicsContext;
	public:
		IGraphicsAPI(CreateInfo info);
		virtual bool Initiate(CreateInfo create_info) = 0;
		virtual bool CleanUp() = 0;

		virtual void Clear() = 0;
		virtual void Present(u8 refresh_rate, u8 flags) = 0;

		virtual void OnAltEnter() = 0;
		virtual void OnResize() = 0;


		std::string GetAPIName() { return m_CreateInfo.m_APIName; }


		virtual void CopyResource(void * pDestination, void * pSource) = 0;

		eGraphicsAPI GetActiveAPI() const { return m_ActiveAPI; }

		virtual void EnableZBuffer() = 0;
		virtual void DisableZBuffer() = 0;


		virtual void* CreateTextureFromFile(const cl::CHashString<128>& str) = 0;
// 
// 		virtual void CreateTexture2D() = 0;
// 		virtual void CreateTexture3D() = 0;


		virtual void SetViewport(void* viewport) = 0;
		virtual void* CreateViewport(u16 width, u16 height, float min_depth, float max_depth, u16 top_left_x, u16 top_left_y) = 0;
		/*
			The depth_value variable is what the depth buffer is testing against in that state. 0.0 - 1.0
		*/
		//virtual void SetDepthStencilState(eDepthStencilState depth_stencil_state, s32 depth_value) = 0;

		virtual IGraphicsDevice& GetDevice() { return *m_Device; }
		virtual IGraphicsContext& GetContext() { return *m_Context; }

	protected:
		CreateInfo m_CreateInfo;
		eGraphicsAPI m_ActiveAPI;

		IGraphicsDevice* m_Device = nullptr;
		IGraphicsContext* m_Context = nullptr;

		//IDevice* m_Device = nullptr;


	};

	IGraphicsAPI::IGraphicsAPI(CreateInfo info)
		: m_CreateInfo(info)
	{
	}

};