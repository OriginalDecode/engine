#include "stdafx.h"
#include "DirectX11.h"
#include <Engine/DX11Device.h>
#include <Engine/DX11Context.h>

namespace graphics
{
	void DirectX11::CreateSamplerStates()
	{
		ID3D11Device* pDevice = static_cast<DX11Device*>(m_Device)->GetDevice();
		DX11Context* ctx = static_cast<DX11Context*>(m_Context);

		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;

		samplerDesc.MaxAnisotropy = 1; 
		samplerDesc.MipLODBias = 0.f;

		ID3D11SamplerState* sampler = nullptr;
		HRESULT hr = pDevice->CreateSamplerState(&samplerDesc, &sampler);
		ctx->m_SamplerStates[MSAA_x1] = sampler;
#ifndef _FINAL
		HandleErrors(hr, "Failed to create Sampler");
		SetDebugName(sampler, "MSAA_x1 Sampler");
#endif

		samplerDesc.MaxAnisotropy = 2; 
		hr = pDevice->CreateSamplerState(&samplerDesc, &sampler);
		ctx->m_SamplerStates[MSAA_x2] = sampler;
#ifndef _FINAL
		HandleErrors(hr, "Failed to create Sampler");
		SetDebugName(sampler, "MSAA_x2 Sampler");
#endif
		samplerDesc.MaxAnisotropy = 4;
		hr = pDevice->CreateSamplerState(&samplerDesc, &sampler);
		ctx->m_SamplerStates[MSAA_x4] = sampler;
#ifndef _FINAL
		HandleErrors(hr, "Failed to create Sampler");
		SetDebugName(sampler, "MSAA_x4 Sampler");
#endif
		samplerDesc.MaxAnisotropy = 8;
		hr = pDevice->CreateSamplerState(&samplerDesc, &sampler);
		ctx->m_SamplerStates[MSAA_x8] = sampler;
#ifndef _FINAL
		HandleErrors(hr, "Failed to create Sampler");
		SetDebugName(sampler, "MSAA_x8 Sampler");
#endif
		samplerDesc.MaxAnisotropy = 16;
		hr = pDevice->CreateSamplerState(&samplerDesc, &sampler);
		ctx->m_SamplerStates[MSAA_x16] = sampler;
#ifndef _FINAL
		HandleErrors(hr, "Failed to create Sampler");
		SetDebugName(sampler, "MSAA_x16 Sampler");
#endif

		samplerDesc = D3D11_SAMPLER_DESC();
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.f;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hr = pDevice->CreateSamplerState(&samplerDesc, &sampler);
		ctx->m_SamplerStates[LINEAR_WRAP] = sampler;
#ifndef _FINAL
		HandleErrors(hr, "Failed to create Sampler");
		SetDebugName(sampler, "Linear Wrap Sampler");
#endif
		samplerDesc = D3D11_SAMPLER_DESC();
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		samplerDesc.MipLODBias = 2.f;
		samplerDesc.MaxAnisotropy = 1;

		hr = pDevice->CreateSamplerState(&samplerDesc, &sampler);
		ctx->m_SamplerStates[LINEAR_CLAMP] = sampler;
#ifndef _FINAL
		HandleErrors(hr, "Failed to create Sampler");
		SetDebugName(sampler, "Linear Clamp Sampler");
#endif

		//~=matrix;

		samplerDesc = D3D11_SAMPLER_DESC();
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		samplerDesc.MipLODBias = 2.f;
		samplerDesc.MaxAnisotropy = 1;

		hr = pDevice->CreateSamplerState(&samplerDesc, &sampler);
		ctx->m_SamplerStates[BILINEAR] = sampler;
#ifndef _FINAL
		HandleErrors(hr, "Failed to create Sampler");
		SetDebugName(sampler, "Linear Clamp Sampler");
#endif



		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MinLOD = -FLT_MAX;
		samplerDesc.MaxLOD = FLT_MAX;
		samplerDesc.MipLODBias = 0.f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

		hr = pDevice->CreateSamplerState(&samplerDesc, &sampler);
		ctx->m_SamplerStates[CUBEMAP] = sampler;
#ifndef _FINAL
		HandleErrors(hr, "Failed to create Sampler");
		SetDebugName(sampler, "Cubemap Sampler");
#endif


		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = FLT_MAX;
		samplerDesc.MipLODBias = 0.f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

		hr = pDevice->CreateSamplerState(&samplerDesc, &sampler);
		ctx->m_SamplerStates[POINT_CLAMP] = sampler;
#ifndef _FINAL
		HandleErrors(hr, "Failed to create Sampler");
		SetDebugName(sampler, "point clamp Sampler");
#endif

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = FLT_MAX;
		samplerDesc.MipLODBias = 0.f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

		hr = pDevice->CreateSamplerState(&samplerDesc, &sampler);
		ctx->m_SamplerStates[POINT_CLAMP] = sampler;
#ifndef _FINAL
		HandleErrors(hr, "Failed to create Sampler");
		SetDebugName(sampler, "point wrap Sampler");
#endif
	}
};