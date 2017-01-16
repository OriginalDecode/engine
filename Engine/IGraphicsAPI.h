#pragma once
#include "snowblind_shared.h"
#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif

struct CreateInfo
{
	HWND m_HWND;
	HINSTANCE m_Instance;
	float m_WindowWidth = 0.f;
	float m_WindowHeight = 0.f;
	std::string m_APIName;
};

enum class eEngineFlags
{
	FULLSCREEN,
	_COUNT
};

enum class eDepthStencilState
{
	Z_ENABLED,
	Z_DISABLED,
	READ_NO_WRITE,
	MASK_TEST,
	LIGHT_MASK,
	DEPTH_TEST,
	_COUNT
};

enum class eRasterizer
{
	WIREFRAME,
	CULL_BACK,
	CULL_NONE,
	CULL_FRONT,
	MSAA,
	_COUNT
};

enum class eBlendStates
{
	NO_BLEND,
	LIGHT_BLEND,
	ALPHA_BLEND,
	BLEND_FALSE,
	_COUNT
};

enum class eSamplerStates
{
	LINEAR_CLAMP,
	LINEAR_WRAP,
	POINT_CLAMP,
	POINT_WRAP,
	NONE,
	_COUNT
};

enum class eGraphicsAPI
{
	D3D11,
	VULKAN,
};

namespace Hex
{
	class IGraphicsAPI
	{
	public:
		virtual bool Initiate(CreateInfo create_info) = 0;
		virtual bool CleanUp() = 0;

		virtual void Clear() = 0;
		virtual void Present(u8 refresh_rate, u8 flags) = 0;

		virtual void OnAltEnter() = 0;
		std::string GetAPIName() { return m_CreateInfo.m_APIName; }

		virtual void EnableZBuffer() = 0;
		virtual void DisableZBuffer() = 0;
		virtual IDevice* GetDevice() = 0;

		virtual void CopyResource(void * pDestination, void * pSource) = 0;

		virtual void SetDebugName(void * pResource, std::string debug_name) = 0;
		eGraphicsAPI GetActiveAPI() const { return m_ActiveAPI; }

		/*
		vulkan has
		Vertex = Vertex
		Fragment = Pixel
		Geometry = Geometry
		Compute = Compute
		Tesselation Control = Hull
		Tesselation Evaluation = Domain
		*/
		virtual void SetVertexShader(void * vertex_shader) = 0;
		virtual void SetPixelShader(void * vertex_shader) = 0;
		virtual void SetGeometryShader(void * vertex_shader) = 0;
		virtual void SetHullShader(void * vertex_shader) = 0;
		virtual void SetDomainShader(void * vertex_shader) = 0;
		virtual void SetComputeShader(void * vertex_shader) = 0;

		/*
			The depth_value variable is what the depth buffer is testing against in that state. 0.0 - 1.0
		*/
		virtual void SetDepthStencilState(eDepthStencilState depth_stencil_state, s32 depth_value) = 0;

	protected:
		CreateInfo m_CreateInfo;
		eGraphicsAPI m_ActiveAPI;


		std::bitset<int(eEngineFlags::_COUNT)> myEngineFlags;
	};

};