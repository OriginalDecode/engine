#pragma once
#include "IGraphicsDevice.h"
#include "IGraphicsAPI.h"
struct ID3D11Device;
namespace graphics
{
	class DX11Device : public IGraphicsDevice
	{
		friend class DirectX11;
	public:
		DX11Device(ID3D11Device* device);
		~DX11Device();

		IVertexShader* CreateVertexShader(IShaderBlob* pShader, const cl::HashString& debug_name) override;
		IPixelShader* CreatePixelShader(IShaderBlob* pShader, const cl::HashString& debug_name) override;
		IGeometryShader* CreateGeometryShader(IShaderBlob* pShader, const cl::HashString& debug_name) override;
		IHullShader* CreateHullShader(IShaderBlob* pShader, const cl::HashString& debug_name) override;
		IDomainShader* CreateDomainShader(IShaderBlob* pShader, const cl::HashString& debug_name) override;
		IComputeShader* CreateComputeShader(IShaderBlob* pShader, const cl::HashString& debug_name) override;
		
		IShaderResourceView* CreateTextureFromFile(const cl::HashString& filepath, bool generate_mips, IGraphicsContext* ctx) override;
		ITexture2D* CreateTexture2D(const Texture2DDesc& desc, const cl::HashString& debug_name) override;
		IRenderTargetView* CreateRenderTarget(const RenderTargetDesc& desc, ITexture2D* pTexture, const cl::HashString& debug_name) override;
		IShaderResourceView* CreateShaderResource(const ShaderResourceDesc& desc, ITexture2D* pTexture, const cl::HashString& debug_name) override;
		IDepthStencilView* CreateDepthStencilView(const DepthDesc& desc, ITexture2D* pTexture, const cl::HashString& debug_name) override;

		IInputLayout* CreateInputLayout(CompiledShader* pShader, void* pLayout, s32 element_count) override;
		IBuffer* CreateBuffer(BufferDesc buffer_desc) override;
			
		IBuffer* CreateConstantBuffer(s32 size) override;
		IShaderBlob* CompileShaderFromFile(const cl::HashString& filepath, const char* entrypoint, const char* shader_type) override;
		IShaderBlob* CompileShaderFromMemory(const s8* pData, s32 data_byte_size, const cl::HashString& source_name, const char* entrypoint, const char* shader_type) override;

	private:
		ID3D11Device* m_Device = nullptr;


	};
};