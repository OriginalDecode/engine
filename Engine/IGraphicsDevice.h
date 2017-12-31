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
		virtual IVertexShader* CreateVertexShader(IShaderBlob* pShader, const std::string& debug_name) = 0;
		virtual IPixelShader* CreatePixelShader(IShaderBlob* pShader, const std::string& debug_name) = 0;
		virtual IGeometryShader* CreateGeometryShader(IShaderBlob* pShader, const std::string& debug_name) = 0;
		virtual IHullShader* CreateHullShader(IShaderBlob* pShader, const std::string& debug_name) = 0;
		virtual IDomainShader* CreateDomainShader(IShaderBlob* pShader, const std::string& debug_name) = 0;
		virtual IComputeShader* CreateComputeShader(IShaderBlob* pShader, const std::string& debug_name) = 0;

		virtual IShaderResourceView* CreateTextureFromFile(const std::string& filepath, bool generate_mips, IGraphicsContext* ctx) = 0;
		virtual IShaderResourceView* CreateTextureFromFile(const char* filepath, ITexture2D*& tex_out, bool generate_mips, IGraphicsContext* ctx) = 0;



		virtual ITexture2D* CreateTexture2D(const Texture2DDesc& desc, const std::string& debug_name) = 0;
		virtual ITexture2D* CreateTexture2D(const Texture2DDesc& desc, s8* data, s32 pitch, const std::string& debug_name) = 0;
		virtual IRenderTargetView* CreateRenderTarget(const Texture2DDesc& desc, ITexture2D* pTexture, const std::string& debug_name) = 0;
		virtual IShaderResourceView* CreateShaderResource(const Texture2DDesc& desc, ITexture2D* pTexture, const std::string& debug_name) = 0;
		virtual IShaderResourceView* CreateShaderResource(ITexture2D* pTexture, const std::string& debug_name) = 0;
		virtual IDepthStencilView* CreateDepthStencilView(const Texture2DDesc& desc, ITexture2D* pTexture, const std::string& debug_name) = 0;

		virtual IInputLayout* CreateInputLayout(CompiledShader* pShader, InputElementDesc* pLayout, const s32 element_count) = 0;
		virtual IBuffer* CreateBuffer(const BufferDesc& buffer_desc, const std::string& debug_name) = 0;

		virtual IBuffer* CreateConstantBuffer(s32 size, const std::string& debug_name) = 0;
		virtual IShaderBlob* CompileShaderFromFile(const std::string& filepath, const char* entrypoint, const char* shader_type) = 0;
		virtual IShaderBlob* CompileShaderFromMemory(const s8* pData, s32 data_byte_size, const std::string& source_name, const char* entrypoint, const char* shader_type) = 0;

		virtual bool FindInputSemantic(const char* to_find, IShaderBlob* blob) = 0;

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
