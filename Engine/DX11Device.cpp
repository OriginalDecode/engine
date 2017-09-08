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

	void* DX11Device::CreateVertexShader(void* pShader, const cl::CHashString<128>& debug_name)
	{
		ID3D11VertexShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifdef _DEBUG
		DL_ASSERT_EXP(hr == S_OK, "Failed to create vertex shader!");
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	void* DX11Device::CreatePixelShader(void* pShader, const cl::CHashString<128>& debug_name)
	{
		ID3D11PixelShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifdef _DEBUG
		DL_ASSERT_EXP(hr == S_OK, "Failed to create pixel shader!");
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	void* DX11Device::CreateGeometryShader(void* pShader, const cl::CHashString<128>& debug_name)
	{
		ID3D11GeometryShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifdef _DEBUG
		DL_ASSERT_EXP(hr == S_OK, "Failed to create geometry shader!");
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	void* DX11Device::CreateHullShader(void* pShader, const cl::CHashString<128>& debug_name)
	{
		ID3D11HullShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifdef _DEBUG
		DL_ASSERT_EXP(hr == S_OK, "Failed to create hull shader!");
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	void* DX11Device::CreateDomainShader(void* pShader, const cl::CHashString<128>& debug_name)
	{
		ID3D11DomainShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifdef _DEBUG
		DL_ASSERT_EXP(hr == S_OK, "Failed to create domain shader!");
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	void* DX11Device::CreateComputeShader(void* pShader, const cl::CHashString<128>& debug_name)
	{
		ID3D11ComputeShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifdef _DEBUG
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
															 , (ID3D11DeviceContext*)ctx->GetContext()
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
		text_desc.Format = GetFormat(desc.m_Format);
		text_desc.SampleDesc.Count = 1;
		text_desc.SampleDesc.Quality = 0;
		text_desc.Usage = GetUsage(desc.m_Usage); //GetUsage
		text_desc.BindFlags = GetBindFlag(desc.m_Binding); //GetBinding
		text_desc.CPUAccessFlags = GetCPUAccessFlag(desc.m_CPUAccessFlag);
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
		view_desc.Format = GetFormat(desc.m_Format);
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
		depth_desc.Format = GetFormat(desc.m_Format);
		depth_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depth_desc.Texture2D.MipSlice = 0;

		ID3D11DepthStencilView* dsv = nullptr;
		ID3D11Texture2D* tex = static_cast<ID3D11Texture2D*>(pTexture);
		HRESULT hr = m_Device->CreateDepthStencilView(tex, &depth_desc, &dsv);
#ifndef FINAL
		DirectX11::HandleErrors(hr, "Failed to create DepthStencilView");
		DirectX11::SetDebugName(dsv, debug_name);
		return static_cast<IDepthStencilView*>(dsv);
#endif

	}

	D3D11_USAGE DX11Device::GetUsage(s32 usage)
	{
		if (usage == DEFAULT_USAGE)
			return D3D11_USAGE_DEFAULT;
		if (usage == IMMUTABLE_USAGE)
			return D3D11_USAGE_IMMUTABLE;
		if (usage == DYNAMIC_USAGE)
			return D3D11_USAGE_DYNAMIC;
		if (usage == STAGING_USAGE)
			return D3D11_USAGE_STAGING;
	}

	u32 DX11Device::GetBindFlag(s32 binding)
	{
		u32 output = 0;

		if (binding & graphics::eTextureBind::SHADERRESOURCE)
			output |= D3D11_BIND_SHADER_RESOURCE;
		if (binding & graphics::eTextureBind::RENDERTARGET)
			output |= D3D11_BIND_RENDER_TARGET;
		if (binding& graphics::eTextureBind::DEPTHSTENCIL)
			output |= D3D11_BIND_DEPTH_STENCIL;

		return output;
	}

	DXGI_FORMAT DX11Device::GetFormat(s32 format)
	{

		//___________________________________________________
		if (format & graphics::eTextureFormat::RGBA32_FLOAT)
			return DXGI_FORMAT_R32G32B32A32_FLOAT;

		if (format & graphics::eTextureFormat::RGBA32_UINT)
			return DXGI_FORMAT_R32G32B32A32_UINT;

		if (format & graphics::eTextureFormat::RGBA32_SINT)
			return DXGI_FORMAT_R32G32B32A32_SINT;


		//___________________________________________________
		if (format & graphics::eTextureFormat::RGB32_FLOAT)
			return DXGI_FORMAT_R32G32B32_FLOAT;

		if (format & graphics::eTextureFormat::RGB32_UINT)
			return DXGI_FORMAT_R32G32B32_UINT;

		if (format & graphics::eTextureFormat::RGB32_SINT)
			return DXGI_FORMAT_R32G32B32_SINT;


		//___________________________________________________
		if (format & graphics::eTextureFormat::RGBA16_FLOAT)
			return DXGI_FORMAT_R16G16B16A16_FLOAT;

		if (format & graphics::eTextureFormat::RGBA16_UINT)
			return DXGI_FORMAT_R16G16B16A16_UINT;

		if (format & graphics::eTextureFormat::RGBA16_SINT)
			return DXGI_FORMAT_R16G16B16A16_SINT;

		//___________________________________________________

		if (format & graphics::eTextureFormat::RGBA8_UINT)
			return DXGI_FORMAT_R8G8B8A8_UINT;

		if (format & graphics::eTextureFormat::RGBA8_SINT)
			return DXGI_FORMAT_R8G8B8A8_SINT;

		//___________________________________________________
		if (format & graphics::eTextureFormat::R32_TYPELESS)
			return DXGI_FORMAT_R32_TYPELESS;

		if (format & graphics::eTextureFormat::R32_FLOAT)
			return DXGI_FORMAT_R32_FLOAT;

		if (format & graphics::eTextureFormat::DEPTH_32_FLOAT)
			return DXGI_FORMAT_D32_FLOAT;


		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	}

	u32 DX11Device::GetCPUAccessFlag(s32 flags)
	{
		u32 output = 0;

		if (flags & eCPUAccessFlag::READ)
			output |= D3D11_CPU_ACCESS_READ;
		if (flags& eCPUAccessFlag::WRITE)
			output |= D3D11_CPU_ACCESS_WRITE;

		return output;
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

	IBuffer* DX11Device::CreateBuffer(void* pBufferDesc)
	{

	}

};