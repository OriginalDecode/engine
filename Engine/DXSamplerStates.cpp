#include "stdafx.h"
#include "DirectX11.h"


void DirectX11::CreateSamplerStates()
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;

	samplerDesc.MaxAnisotropy = 16; //Should be a config number
	samplerDesc.MipLODBias = 0.f;

	myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[u16(eSamplerStates::LINEAR_CLAMP)]);
	SetDebugName(mySamplerStates[u16(eSamplerStates::LINEAR_CLAMP)], "LINEAR_CLAMP SamplerState");

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[u16(eSamplerStates::POINT_CLAMP)]);
	SetDebugName(mySamplerStates[u16(eSamplerStates::POINT_CLAMP)], "POINT_CLAMP SamplerState");


	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[u16(eSamplerStates::LINEAR_WRAP)]);
	SetDebugName(mySamplerStates[u16(eSamplerStates::LINEAR_WRAP)], "LINEAR_WRAP SamplerState");

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[u16(eSamplerStates::POINT_WRAP)]);
	SetDebugName(mySamplerStates[u16(eSamplerStates::POINT_WRAP)], "POINT_WRAP SamplerState");



	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MipLODBias = 0.f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[(u32)eSamplerStates::MIP_SAMPLE]);
	SetDebugName(mySamplerStates[(u32)eSamplerStates::MIP_SAMPLE], "MIP_SAMPLE SamplerState");

}
