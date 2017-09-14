#pragma once
#include "IGraphicsDevice.h"
#include "IGraphicsAPI.h"
struct ID3D11Device;
namespace graphics
{
	class DX11Device : public IGraphicsDevice
	{
	public:
		DX11Device(ID3D11Device* device);


		void* CreateVertexShader(IShaderBlob* pShader, const cl::CHashString<128>& debug_name) override;
		void* CreatePixelShader(IShaderBlob* pShader, const cl::CHashString<128>& debug_name) override;
		void* CreateGeometryShader(IShaderBlob* pShader, const cl::CHashString<128>& debug_name) override;
		void* CreateHullShader(IShaderBlob* pShader, const cl::CHashString<128>& debug_name) override;
		void* CreateDomainShader(IShaderBlob* pShader, const cl::CHashString<128>& debug_name) override;
		void* CreateComputeShader(IShaderBlob* pShader, const cl::CHashString<128>& debug_name) override;
		
		void* CreateTextureFromFile(const cl::CHashString<128>& filepath, bool generate_mips, IGraphicsContext* ctx) override;
		ITexture2D* CreateTexture2D(const Texture2DDesc& desc, const cl::CHashString<128>& debug_name) override;
		IRenderTargetView* CreateRenderTarget(const RenderTargetDesc& desc, ITexture2D* pTexture, const cl::CHashString<128>& debug_name) override;
		IShaderResourceView* CreateShaderResource(const ShaderResourceDesc& desc, ITexture2D* pTexture, const cl::CHashString<128>& debug_name) override;
		IDepthStencilView* CreateDepthStencilView(const DepthDesc& desc, ITexture2D* pTexture, const cl::CHashString<128>& debug_name) override;

		IDepthStencilState* CreateDepthStencilState() override;

		IInputLayout* CreateInputLayout(CompiledShader* pShader, void* pLayout, s32 element_count) override;
		IBuffer* CreateBuffer(BufferDesc buffer_desc) override;


	private:
		ID3D11Device* m_Device = nullptr;


	};
};