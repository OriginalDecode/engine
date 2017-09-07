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
		DL_ASSERT_EXP(hr == S_OK, "Failed to create vertex shader!");
#ifdef _DEBUG
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	void* DX11Device::CreatePixelShader(void* pShader, const cl::CHashString<128>& debug_name)
	{
		ID3D11PixelShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
		DL_ASSERT_EXP(hr == S_OK, "Failed to create pixel shader!");
#ifdef _DEBUG
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	void* DX11Device::CreateGeometryShader(void* pShader, const cl::CHashString<128>& debug_name)
	{
		ID3D11GeometryShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
		DL_ASSERT_EXP(hr == S_OK, "Failed to create geometry shader!");
#ifdef _DEBUG
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	void* DX11Device::CreateHullShader(void* pShader, const cl::CHashString<128>& debug_name)
	{
		ID3D11HullShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
		DL_ASSERT_EXP(hr == S_OK, "Failed to create hull shader!");
#ifdef _DEBUG
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	void* DX11Device::CreateDomainShader(void* pShader, const cl::CHashString<128>& debug_name)
	{
		ID3D11DomainShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
		DL_ASSERT_EXP(hr == S_OK, "Failed to create domain shader!");
#ifdef _DEBUG
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	void* DX11Device::CreateComputeShader(void* pShader, const cl::CHashString<128>& debug_name)
	{
		ID3D11ComputeShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
		DL_ASSERT_EXP(hr == S_OK, "Failed to create compute shader!");
#ifdef _DEBUG
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

			DL_ASSERT_EXP(hr != S_OK, "Failed to load texture");
		}
		else
		{
			HRESULT hr = DirectX::CreateDDSTextureFromFile(m_Device
				, nullptr
				, widepath
				, nullptr //might want to output to a texture2d object?
				, &srv);
			DL_ASSERT_EXP(hr != S_OK, "Failed to load texture");
		}

		return srv;
	}

	void* DX11Device::CreateTexture2D(void* pOutTexture, Texture2DDesc desc, const cl::CHashString<128>& debug_name)
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
		DirectX11::HandleErrors(hr, "Failed to create Texture!");
		

		return nullptr;
	}

	void DX11Device::ReleasePtr(void* ptr)
	{
		IUnknown* pUnknown = static_cast<IUnknown*>(ptr);
		pUnknown->Release();
	}

	D3D11_USAGE DX11Device::GetUsage(s32 usage)
	{

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
		u32 output = 0;
		if(format & graphics::eTextureFormat::RGBA32_FLOAT)

	}

	u32 DX11Device::GetCPUAccessFlag(s32 flags)
	{
		


	}

};