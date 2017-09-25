#include "stdafx.h"
#include "DirectX11.h"
#include <Engine/DX11Device.h>

namespace graphics
{
	void DirectX11::CreateSamplerStates()
	{
		ID3D11Device* pDevice = static_cast<DX11Device*>(m_Device)->m_Device;


		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;

		samplerDesc.MaxAnisotropy = 1; 
		samplerDesc.MipLODBias = 0.f;

		ID3D11SamplerState* sampler = nullptr;
		pDevice->CreateSamplerState(&samplerDesc, &sampler);
		m_SamplerStates[MSAA_x1] = sampler;
#ifndef FINAL
		SetDebugName(sampler, "MSAA_x1 Sampler");
#endif

		samplerDesc.MaxAnisotropy = 2; 
		pDevice->CreateSamplerState(&samplerDesc, &sampler);
		m_SamplerStates[MSAA_x2] = sampler;
#ifndef FINAL
		SetDebugName(sampler, "MSAA_x2 Sampler");
#endif
		samplerDesc.MaxAnisotropy = 4;
		pDevice->CreateSamplerState(&samplerDesc, &sampler);
		m_SamplerStates[MSAA_x4] = sampler;
#ifndef FINAL
		SetDebugName(sampler, "MSAA_x4 Sampler");
#endif
		samplerDesc.MaxAnisotropy = 8;
		pDevice->CreateSamplerState(&samplerDesc, &sampler);
		m_SamplerStates[MSAA_x8] = sampler;
#ifndef FINAL
		SetDebugName(sampler, "MSAA_x8 Sampler");
#endif
		samplerDesc.MaxAnisotropy = 16;
		pDevice->CreateSamplerState(&samplerDesc, &sampler);
		m_SamplerStates[MSAA_x16] = sampler;
#ifndef FINAL
		SetDebugName(sampler, "MSAA_x16 Sampler");
#endif




		// 	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		// 	myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[u16(eSamplerStates::POINT_CLAMP)]);
		// 	SetDebugName(mySamplerStates[u16(eSamplerStates::POINT_CLAMP)], "POINT_CLAMP SamplerState");
		// 
		// 
		// 	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		// 	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		// 	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		// 
		// 	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		// 	myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[u16(eSamplerStates::LINEAR_WRAP)]);
		// 	SetDebugName(mySamplerStates[u16(eSamplerStates::LINEAR_WRAP)], "LINEAR_WRAP SamplerState");
		// 
		// 	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		// 	myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[u16(eSamplerStates::POINT_WRAP)]);
		// 	SetDebugName(mySamplerStates[u16(eSamplerStates::POINT_WRAP)], "POINT_WRAP SamplerState");
	}
};