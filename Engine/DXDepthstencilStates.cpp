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

	void DirectX11::CreateDepthStencilStates()
	{
		DX11Device* dx11device = static_cast<DX11Device*>(m_Device);
		ID3D11Device* device = dx11device->GetDevice();
		DX11Context* ctx = static_cast<DX11Context*>(m_Context);

		//__________________________________________________________
		ID3D11DepthStencilState* depthstencil = nullptr;
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

		HRESULT hr = device->CreateDepthStencilState(&stencilDesc, &depthstencil);
		ctx->m_DepthStencilStates[Z_ENABLED] = depthstencil;
#ifndef FINAL
		SetDebugName(depthstencil, "DepthstencilState Z_Enable");
		HandleErrors(hr, "Failed to create DepthStencilState!");
#endif
		depthstencil = nullptr;

		//__________________________________________________________

		stencilDesc.DepthEnable = false;
		 
		hr = device->CreateDepthStencilState(&stencilDesc, &depthstencil);
		ctx->m_DepthStencilStates[Z_DISABLED] = depthstencil;
#ifndef FINAL
		SetDebugName(depthstencil, "DepthstencilState Z_Disable");
		HandleErrors(hr, "Failed to create DepthStencilState!");
#endif
		depthstencil = nullptr;

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

		hr = device->CreateDepthStencilState(&stencilDesc, &depthstencil);
		ctx->m_DepthStencilStates[READ_NO_WRITE] = depthstencil;
#ifndef FINAL
		SetDebugName(depthstencil, "DepthstencilState ReadNoWrite");
		HandleErrors(hr, "Failed to create DepthStencilState!");
#endif 
		depthstencil = nullptr;

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

		hr = device->CreateDepthStencilState(&stencilDesc, &depthstencil);
		ctx->m_DepthStencilStates[READ_NO_WRITE_PARTICLE] = depthstencil;
#ifndef FINAL
		SetDebugName(depthstencil, "DepthStencilState ReadNoWriteParticle");
		HandleErrors(hr, "Failed to create DepthStencilState!");
#endif
		depthstencil = nullptr;

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

		hr = device->CreateDepthStencilState(&stencilDesc, &depthstencil);
		ctx->m_DepthStencilStates[Z_EQUAL] = depthstencil;
#ifndef FINAL
		SetDebugName(depthstencil, "DepthstencilState Z_EQUAL");
		HandleErrors(hr, "Failed to create DepthStencilState!");
#endif
		depthstencil = nullptr;



	}
};