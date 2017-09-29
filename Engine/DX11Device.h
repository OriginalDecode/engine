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
		ITexture2D* CreateTexture2D(const Texture2DDesc& desc, s8* data, s32 pitch, const cl::HashString& debug_name) override;
		IRenderTargetView* CreateRenderTarget(const Texture2DDesc& desc, ITexture2D* pTexture, const cl::HashString& debug_name) override;
		IShaderResourceView* CreateShaderResource(const Texture2DDesc& desc, ITexture2D* pTexture, const cl::HashString& debug_name) override;
		IShaderResourceView* CreateShaderResource(ITexture2D* pTexture, const cl::HashString& debug_name) override;
		IDepthStencilView* CreateDepthStencilView(const Texture2DDesc& desc, ITexture2D* pTexture, const cl::HashString& debug_name) override;

		IInputLayout* CreateInputLayout(CompiledShader* pShader, InputElementDesc* pLayout, const s32 element_count) override;
		IBuffer* CreateBuffer(const BufferDesc& buffer_desc, const cl::HashString& debug_name) override;
			
		IBuffer* CreateConstantBuffer(s32 size, const cl::HashString& debug_name) override;
		IShaderBlob* CompileShaderFromFile(const cl::HashString& filepath, const char* entrypoint, const char* shader_type) override;
		IShaderBlob* CompileShaderFromMemory(const s8* pData, s32 data_byte_size, const cl::HashString& source_name, const char* entrypoint, const char* shader_type) override;


	private:
		ID3D11Device* m_Device = nullptr;


	};
};