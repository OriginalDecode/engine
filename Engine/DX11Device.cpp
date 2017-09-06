#include "stdafx.h"
#include "DX11Device.h"


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

};