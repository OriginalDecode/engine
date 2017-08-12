#pragma once
#include "engine_shared.h"
#include <Engine/ShaderState.h>
#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif

// typedef void** GTexture2D;
// typedef void** GTexture3D;
// typedef void** GShaderResourceView;
// typedef void** GDepthStencilView;
// typedef void** GRenderTargetView;
// typedef void** GViewport;
// typedef void** GVertexShader;
// typedef void** GPixelShader;
// typedef void** GGeometryShader;
// typedef void** GHullShader;
// typedef void** GDomainShader;
// typedef void** GComputeShader;
// typedef void** GShaderBlob;
// typedef void** GDevice;
// typedef void** GContext;
// typedef void** GBuffer;
// typedef void** GInputLayout;


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
	READ_NO_WRITE_PARTICLE,
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
	PARTICLE_BLEND,
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
	NONE = -1,
	D3D11,
	VULKAN,
};


class IGraphicsAPI
{
public:
	virtual bool Initiate(CreateInfo create_info) = 0;
	virtual bool CleanUp() = 0;

	virtual void Clear() = 0;
	virtual void Present(u8 refresh_rate, u8 flags) = 0;

	virtual void OnAltEnter() = 0;
	virtual void OnResize() = 0;


	std::string GetAPIName() { return m_CreateInfo.m_APIName; }

	virtual IDevice* GetDevice() = 0;

	virtual void CopyResource(void * pDestination, void * pSource) = 0;

	virtual void SetDebugName(void * pResource, std::string debug_name) = 0;
	eGraphicsAPI GetActiveAPI() const { return m_ActiveAPI; }

	virtual void EnableZBuffer() = 0;
	virtual void DisableZBuffer() = 0;


	virtual void* CreateBlendState(s32 render_target_write_mask
		, s32 enable_blend_flags
		, BlendState::BlendOp blend_op, BlendState::BlendFlag src_blend, BlendState::BlendFlag dest_blend
		, BlendState::BlendOp alpha_blend_op, BlendState::BlendFlag src_blend_alpha, BlendState::BlendFlag dest_blend_alpha) = 0;
	virtual void* CreateSamplerState(SamplerState::FilterMode filter_mode, SamplerState::UVAddressMode address_mode, u32 max_anisotropy, float mip_lod_bias, float min_lod, float max_lod, float border_color[4], SamplerState::ComparisonFunc comparison_function) = 0;

	virtual void* CreateRasterizerState() = 0;
	virtual void* CreateDepthstencilState() = 0;




	/*
	vulkan has
	Vertex = Vertex
	Fragment = Pixel
	Geometry = Geometry
	Compute = Compute
	Tesselation Control = Hull
	Tesselation Evaluation = Domain
	*/
	virtual void SetVertexShader(CompiledShader* vertex_shader) = 0;
	virtual void SetPixelShader(CompiledShader* vertex_shader) = 0;
	virtual void SetGeometryShader(CompiledShader* vertex_shader) = 0;
	virtual void SetHullShader(CompiledShader* vertex_shader) = 0;
	virtual void SetDomainShader(CompiledShader* vertex_shader) = 0;
	virtual void SetComputeShader(CompiledShader* vertex_shader) = 0;

	virtual void* CreateVertexShader(void* pBuffer, float buffer_size) = 0;
	virtual void* CreatePixelShader(void* pBuffer, float buffer_size) = 0;
	virtual void* CreateGeometryShader(void* pBuffer, float buffer_size) = 0;
	virtual void* CreateHullShader(void* pBuffer, float buffer_size) = 0;
	virtual void* CreateDomainShader(void* pBuffer, float buffer_size) = 0;
	virtual void* CreateComputeShader(void* pBuffer, float buffer_size) = 0;

	virtual void SetShaderState(ShaderState& shader_state) = 0;


	virtual void VSSetShaderResource(s32 start_slot, s32 count, void* resources) = 0;
	virtual void PSSetShaderResource(s32 start_slot, s32 count, void* resources) = 0;
	virtual void GSSetShaderResource(s32 start_slot, s32 count, void* resources) = 0;
	virtual void DSSetShaderResource(s32 start_slot, s32 count, void* resources) = 0;
	virtual void HSSetShaderResource(s32 start_slot, s32 count, void* resources) = 0;
	virtual void CSSetShaderResource(s32 start_slot, s32 count, void* resources) = 0;



	/*
		The depth_value variable is what the depth buffer is testing against in that state. 0.0 - 1.0
	*/
	virtual void SetDepthStencilState(eDepthStencilState depth_stencil_state, s32 depth_value) = 0;

protected:
	CreateInfo m_CreateInfo;
	eGraphicsAPI m_ActiveAPI;

	std::bitset<int(eEngineFlags::_COUNT)> myEngineFlags;
};
