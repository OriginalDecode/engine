#include "stdafx.h"

#include "DX11Device.h"
#include <Engine/DX11Context.h>

#include <DDSTextureLoader.h>


namespace graphics
{

	DX11Device::DX11Device(ID3D11Device* device)
		: m_Device(device)
	{
	}

	void* DX11Device::CreateVertexShader(IShaderBlob* pShader, const cl::CHashString<128>& debug_name)
	{
		ID3D11VertexShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifndef FINAL
		DL_ASSERT_EXP(hr == S_OK, "Failed to create vertex shader!");
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	void* DX11Device::CreatePixelShader(IShaderBlob* pShader, const cl::CHashString<128>& debug_name)
	{
		ID3D11PixelShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifndef FINAL
		DL_ASSERT_EXP(hr == S_OK, "Failed to create pixel shader!");
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	void* DX11Device::CreateGeometryShader(IShaderBlob* pShader, const cl::CHashString<128>& debug_name)
	{
		ID3D11GeometryShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifndef FINAL
		DL_ASSERT_EXP(hr == S_OK, "Failed to create geometry shader!");
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	void* DX11Device::CreateHullShader(IShaderBlob* pShader, const cl::CHashString<128>& debug_name)
	{
		ID3D11HullShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifndef FINAL
		DL_ASSERT_EXP(hr == S_OK, "Failed to create hull shader!");
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	void* DX11Device::CreateDomainShader(IShaderBlob* pShader, const cl::CHashString<128>& debug_name)
	{
		ID3D11DomainShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifndef FINAL
		DL_ASSERT_EXP(hr == S_OK, "Failed to create domain shader!");
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	void* DX11Device::CreateComputeShader(IShaderBlob* pShader, const cl::CHashString<128>& debug_name)
	{
		ID3D11ComputeShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifndef FINAL
		DL_ASSERT_EXP(hr == S_OK, "Failed to create compute shader!");
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	void* DX11Device::CreateTextureFromFile(const cl::CHashString<128>& filepath, bool generate_mips, IGraphicsContext* ctx)
	{
		wchar_t* widepath = nullptr;
		mbstowcs(widepath, filepath.c_str(), filepath.length());
		ID3D11ShaderResourceView* srv = nullptr;
		if (generate_mips)
		{
			HRESULT hr = DirectX::CreateDDSTextureFromFileEx(m_Device
															 , static_cast<ID3D11DeviceContext*>(ctx->GetContext())
															 , widepath
															 , 0
															 , D3D11_USAGE_DEFAULT
															 , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET //has to be bound as a render target to actually generate the mips
															 , D3D11_CPU_ACCESS_READ
															 , D3D11_RESOURCE_MISC_GENERATE_MIPS
															 , false
															 , nullptr //might want to output to a texture2d object?
															 , &srv);

#ifndef FINAL
			DL_ASSERT_EXP(hr != S_OK, "Failed to load texture");
#endif
		}
		else
		{
			HRESULT hr = DirectX::CreateDDSTextureFromFile(m_Device
														   , nullptr
														   , widepath
														   , nullptr //might want to output to a texture2d object?
														   , &srv);
#ifndef FINAL
			DL_ASSERT_EXP(hr != S_OK, "Failed to load texture");
#endif
		}

		return srv;
	}

	ITexture2D* DX11Device::CreateTexture2D(const Texture2DDesc& desc, const cl::CHashString<128>& debug_name)
	{
		D3D11_TEXTURE2D_DESC text_desc;
		text_desc.Width = desc.m_Width;
		text_desc.Height = desc.m_Height;
		text_desc.MipLevels = 1;
		text_desc.Format = DirectX11::GetFormat(desc.m_Format);
		text_desc.SampleDesc.Count = 1;
		text_desc.SampleDesc.Quality = 0;
		text_desc.Usage = DirectX11::GetUsage(desc.m_Usage); //GetUsage
		text_desc.BindFlags = DirectX11::GetBindFlag(desc.m_Binding); //GetBinding
		text_desc.CPUAccessFlags = DirectX11::GetCPUAccessFlag(desc.m_CPUAccessFlag);
		text_desc.MiscFlags = 0;
		text_desc.ArraySize = 1;

		ID3D11Texture2D* texture = nullptr;
		HRESULT hr = m_Device->CreateTexture2D(&text_desc, NULL, &texture);
#ifndef FINAL
		DirectX11::HandleErrors(hr, "Failed to create Texture!");
		DirectX11::SetDebugName(texture, debug_name);
#endif
		return static_cast<ITexture2D*>(texture);
	}

	IRenderTargetView* DX11Device::CreateRenderTarget(const RenderTargetDesc& desc, ITexture2D* pTexture, const cl::CHashString<128>& debug_name)
	{
		ID3D11RenderTargetView* rtv = nullptr;
		ID3D11Texture2D* tex = static_cast<ID3D11Texture2D*>(pTexture);
		HRESULT hr = m_Device->CreateRenderTargetView(tex, nullptr, &rtv);
#ifndef FINAL
		DirectX11::HandleErrors(hr, "Failed to create RenderTargetView");
		DirectX11::SetDebugName(rtv, debug_name);
#endif
		return static_cast<IRenderTargetView*>(rtv);
	}

	IShaderResourceView* DX11Device::CreateShaderResource(const ShaderResourceDesc& desc, ITexture2D* pTexture, const cl::CHashString<128>& debug_name)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
		view_desc.Format = DirectX11::GetFormat(desc.m_Format);
		view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		view_desc.Texture2D.MipLevels = 1;
		view_desc.Texture2D.MostDetailedMip = 0;

		ID3D11ShaderResourceView* srv = nullptr;
		ID3D11Texture2D* tex = static_cast<ID3D11Texture2D*>(pTexture);
		HRESULT hr = m_Device->CreateShaderResourceView(tex, &view_desc, &srv);
#ifndef FINAL
		DirectX11::HandleErrors(hr, "Failed to create ShaderResourceView");
		DirectX11::SetDebugName(srv, debug_name);
#endif
		return static_cast<IShaderResourceView*>(srv);
	}

	IDepthStencilView* DX11Device::CreateDepthStencilView(const DepthDesc& desc, ITexture2D* pTexture, const cl::CHashString<128>& debug_name)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depth_desc;
		depth_desc.Format = DirectX11::GetFormat(desc.m_Format);
		depth_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depth_desc.Texture2D.MipSlice = 0;

		ID3D11DepthStencilView* dsv = nullptr;
		ID3D11Texture2D* tex = static_cast<ID3D11Texture2D*>(pTexture);
		HRESULT hr = m_Device->CreateDepthStencilView(tex, &depth_desc, &dsv);
#ifndef FINAL
		DirectX11::HandleErrors(hr, "Failed to create DepthStencilView");
		DirectX11::SetDebugName(dsv, debug_name);
#endif
		return static_cast<IDepthStencilView*>(dsv);
	}

	IInputLayout* DX11Device::CreateInputLayout(CompiledShader* pShader, void* pLayout, s32 element_count)
	{
		ID3D11InputLayout* layout = nullptr;

		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader->m_Blob);

		HRESULT hr = m_Device->CreateInputLayout(static_cast<D3D11_INPUT_ELEMENT_DESC*>(pLayout), element_count, blob->GetBufferPointer(), blob->GetBufferSize(), &layout);
#ifndef FINAL
		DirectX11::HandleErrors(hr, "Failed to create InputLayout");
		DirectX11::SetDebugName(layout, "InputLayout");
#endif
		return static_cast<IInputLayout*>(layout);
	}

	IBuffer* DX11Device::CreateBuffer(BufferDesc buffer_desc)
	{
		ID3D11Buffer* buffer = nullptr;
		D3D11_BUFFER_DESC desc;
		desc.BindFlags = buffer_desc.m_BindFlag;
		desc.CPUAccessFlags = DirectX11::GetCPUAccessFlag(buffer_desc.m_CPUAccessFlag);
		desc.Usage = DirectX11::GetUsage(buffer_desc.m_UsageFlag);
		desc.MiscFlags = buffer_desc.m_MiscFlags;
		desc.StructureByteStride = buffer_desc.m_StructuredByteStride;
		desc.ByteWidth = buffer_desc.m_ByteWidth;


		D3D11_SUBRESOURCE_DATA srd;
		HRESULT hr = S_OK;

		if (buffer_desc.m_Data)
		{
			srd.pSysMem = buffer_desc.m_Data;
			hr = m_Device->CreateBuffer(&desc, &srd, &buffer);
		}
		else
		{
			DL_WARNING("subresource data is null, this is fine, but the developer might not have intended for it to be null.");
			hr = m_Device->CreateBuffer(&desc, nullptr, &buffer);
		}

#ifndef FINAL
		DirectX11::HandleErrors(hr, "Failed to create buffer!");
		DirectX11::SetDebugName(0, "Unknown_Buffer");
#endif

		return buffer;
	}

};