#pragma once
#include "IGraphicsDevice.h"
#include "IGraphicsAPI.h"
#include <tuple>
struct ID3D11Device;
namespace graphics
{
	class DirectX11;
	class DX11Device : public IGraphicsDevice
	{
	public:
		DX11Device(ID3D11Device* device);
		~DX11Device();

		IVertexShader* CreateVertexShader(IShaderBlob* pShader, const std::string& debug_name) override;
		IFragmentShader* CreatePixelShader(IShaderBlob* pShader, const std::string& debug_name) override;
		IGeometryShader* CreateGeometryShader(IShaderBlob* pShader, const std::string& debug_name) override;
		IHullShader* CreateHullShader(IShaderBlob* pShader, const std::string& debug_name) override;
		IDomainShader* CreateDomainShader(IShaderBlob* pShader, const std::string& debug_name) override;
		IComputeShader* CreateComputeShader(IShaderBlob* pShader, const std::string& debug_name) override;
		
		std::tuple<IShaderResourceView*, ITexture2D*> CreateTextureFromFile(const std::string& filepath, IGraphicsContext* ctx);

		IShaderResourceView* CreateTextureFromFile(const std::string& filepath, bool generate_mips, IGraphicsContext* ctx) override;
		IShaderResourceView* CreateTextureFromFile(const std::string& filepath, ITexture2D*& tex_out, bool generate_mips, IGraphicsContext* ctx) override;

		ITexture2D* CreateTexture2D(const Texture2DDesc& desc, const std::string& debug_name) override;
		ITexture2D* CreateTexture2D(const Texture2DDesc& desc, int8* data, int32 pitch, const std::string& debug_name) override;
		IRenderTargetView* CreateRenderTarget(const Texture2DDesc& desc, ITexture2D* pTexture, const std::string& debug_name) override;
		IShaderResourceView* CreateShaderResource(const Texture2DDesc& desc, ITexture2D* pTexture, const std::string& debug_name) override;
		IShaderResourceView* CreateShaderResource(ITexture2D* pTexture, const std::string& debug_name) override;
		IDepthStencilView* CreateDepthStencilView(const Texture2DDesc& desc, ITexture2D* pTexture, const std::string& debug_name) override;

		IInputLayout* CreateInputLayout(CompiledShader* pShader, const InputElementDesc* const pLayout, const int32 element_count) override;
		IInputLayout* CreateInputLayout(CompiledShader* vertex_shader, const SInputLayout& layout) override;

		IBuffer* CreateBuffer(const BufferDesc& buffer_desc, const std::string& debug_name) override;
			
		IBuffer* CreateConstantBuffer(int32 size, const std::string& debug_name) override;
		IShaderBlob* CompileShaderFromFile(const std::string& filepath, const char* entrypoint, const char* shader_type) override;
		IShaderBlob* CompileShaderFromMemory(const int8* pData, int32 data_byte_size, const std::string& source_name, const char* entrypoint, const char* shader_type) override;

		bool FindInputSemantic(const char* to_find, IShaderBlob* blob) override;

		ID3D11Device* GetDevice() { return m_Device; }

	private:
		ID3D11Device* m_Device = nullptr;


	};
};