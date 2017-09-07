#pragma once
#include <CommonLib/HashString.h>

#include <Engine/engine_shared.h>
#include "IGraphicsAPI.h"
namespace graphics
{
	class IGraphicsContext;
	class IGraphicsDevice
	{
	public:
		virtual void* CreateVertexShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;
		virtual void* CreatePixelShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;
		virtual void* CreateGeometryShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;
		virtual void* CreateHullShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;
		virtual void* CreateDomainShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;
		virtual void* CreateComputeShader(void* pShader, const cl::CHashString<128>& debug_name) = 0;

		virtual void* CreateTextureFromFile(const cl::CHashString<128>& filepath, bool generate_mips, IGraphicsContext* ctx) = 0;
		virtual ITexture2D* CreateTexture2D(const Texture2DDesc& desc, const cl::CHashString<128>& debug_name) = 0;
		virtual IRenderTargetView* CreateRenderTarget(const RenderTargetDesc& desc, ITexture2D* pTexture, const cl::CHashString<128>& debug_name) = 0;
		virtual IShaderResourceView* CreateShaderResource(const ShaderResourceDesc& desc, ITexture2D* pTexture, const cl::CHashString<128>& debug_name) = 0;
		virtual IDepthStencilView* CreateDepthStencilView(const DepthDesc& desc, ITexture2D* pTexture, const cl::CHashString<128>& debug_name) = 0;


		virtual void ReleasePtr(void* ptr) = 0;

	private:
	};
};

/*
MAPPING FOR VULKAN
Vertex = Vertex
Fragment = Pixel
Geometry = Geometry
Compute = Compute
tessellation Control = Hull
tessellation Evaluation = Domain
*/
