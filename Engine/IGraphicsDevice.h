#pragma once
#include <CommonLib/HashString.h>

#include <Engine/engine_shared.h>
#include <Engine/IGraphicsAPI.h>
namespace graphics
{
	class IGraphicsContext;
	class IGraphicsDevice
	{
	public:
		virtual IVertexShader* CreateVertexShader(IShaderBlob* pShader, const cl::HashString& debug_name) = 0;
		virtual IPixelShader* CreatePixelShader(IShaderBlob* pShader, const cl::HashString& debug_name) = 0;
		virtual IGeometryShader* CreateGeometryShader(IShaderBlob* pShader, const cl::HashString& debug_name) = 0;
		virtual IHullShader* CreateHullShader(IShaderBlob* pShader, const cl::HashString& debug_name) = 0;
		virtual IDomainShader* CreateDomainShader(IShaderBlob* pShader, const cl::HashString& debug_name) = 0;
		virtual IComputeShader* CreateComputeShader(IShaderBlob* pShader, const cl::HashString& debug_name) = 0;

		virtual void* CreateTextureFromFile(const cl::HashString& filepath, bool generate_mips, IGraphicsContext* ctx) = 0;
		virtual ITexture2D* CreateTexture2D(const Texture2DDesc& desc, const cl::HashString& debug_name) = 0;
		virtual IRenderTargetView* CreateRenderTarget(const RenderTargetDesc& desc, ITexture2D* pTexture, const cl::HashString& debug_name) = 0;
		virtual IShaderResourceView* CreateShaderResource(const ShaderResourceDesc& desc, ITexture2D* pTexture, const cl::HashString& debug_name) = 0;
		virtual IDepthStencilView* CreateDepthStencilView(const DepthDesc& desc, ITexture2D* pTexture, const cl::HashString& debug_name) = 0;

		virtual IInputLayout* CreateInputLayout(CompiledShader* pShader, void* pLayout, s32 element_count) = 0;
		virtual IBuffer* CreateBuffer(BufferDesc buffer_desc) = 0;

		virtual IBuffer* CreateConstantBuffer(s32 size) = 0;



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
