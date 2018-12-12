#include "stdafx.h"
#include "DirectX11.h"
#include <Engine/DX11Device.h>
#include <Engine/DX11Context.h>
namespace graphics
{

	// DirectX11 depthdesc
	// 	BOOL DepthEnable;
	// 	D3D11_DEPTH_WRITE_MASK DepthWriteMask;
	// 	D3D11_COMPARISON_FUNC DepthFunc;
	// 	BOOL StencilEnable;
	// 	UINT8 StencilReadMask;
	// 	UINT8 StencilWriteMask;
	// 	D3D11_DEPTH_STENCILOP_DESC FrontFace;
	// 	D3D11_DEPTH_STENCILOP_DESC BackFace;

	void DirectX11::CreateDepthStencil(int32 slot, const D3D11_DEPTH_STENCIL_DESC& desc, const std::string& debug_name)
	{

		DX11Device* dx11device = static_cast<DX11Device*>(m_Device);
		ID3D11Device* device = dx11device->GetDevice();
		DX11Context* ctx = static_cast<DX11Context*>(m_Context);
		ID3D11DepthStencilState* depthstencil = nullptr;

		HRESULT hr = device->CreateDepthStencilState(&desc, &depthstencil);
		ctx->m_DepthStencilStates[slot] = depthstencil;
#ifndef FINAL
		std::string depth_stencil_debug_name = "DepthstencilState";
		depth_stencil_debug_name += debug_name;

		SetDebugName(depthstencil, depth_stencil_debug_name);
		HandleErrors(hr, "Failed to create DepthStencilState!");
#endif

	}

	void DirectX11::CreateDepthStencilStates()
	{
		D3D11_DEPTH_STENCIL_DESC  stencilDesc;
		ZeroMemory(&stencilDesc, sizeof(stencilDesc));

		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		stencilDesc.StencilEnable = true;
		stencilDesc.StencilReadMask = 0xFF;
		stencilDesc.StencilWriteMask = 0xFF;

		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		
		CreateDepthStencil(Z_ENABLED, stencilDesc, "Z_Enable");


		//__________________________________________________________

		stencilDesc.DepthEnable = false;
		CreateDepthStencil(Z_DISABLED, stencilDesc, "Z_Disable");

		//__________________________________________________________

		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;

		stencilDesc.StencilEnable = true;
		stencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		stencilDesc.StencilWriteMask = 0x0;

		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
		
		CreateDepthStencil(READ_NO_WRITE, stencilDesc, "ReadNoWrite");

		//__________________________________________________________

		
		stencilDesc.DepthEnable = TRUE;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		stencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		stencilDesc.StencilEnable = FALSE;
		stencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		stencilDesc.StencilWriteMask = 0x0;

		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
		CreateDepthStencil(READ_NO_WRITE_PARTICLE, stencilDesc, "ReadNoWriteParticle");



		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		stencilDesc.DepthFunc = D3D11_COMPARISON_EQUAL;

		stencilDesc.StencilEnable = true;
		stencilDesc.StencilReadMask = 0xFF;
		stencilDesc.StencilWriteMask = 0xFF;

		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		CreateDepthStencil(Z_EQUAL, stencilDesc, "Z_EQUAL");



		stencilDesc.DepthEnable = FALSE;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;

		stencilDesc.StencilEnable = TRUE;
		stencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		stencilDesc.StencilWriteMask = 0x0;

		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER;
		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;

		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		CreateDepthStencil(Z_READ_NO_WRITE, stencilDesc, "ZReadNoWrite");


	}
};