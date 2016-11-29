#include "stdafx.h"
#include "DirectX11.h"
#ifdef SNOWBLIND_DX11
namespace Snowblind
{
	void DirectX11::CreateEnabledDepthStencilState()
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

		HRESULT hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStates[u16(eDepthStencil::Z_ENABLED)]);
		SetDebugName(myDepthStates[u16(eDepthStencil::Z_ENABLED)], "eDepthStencil::Z_ENABLED");
		HandleErrors(hr, "Failed to setup Enabled Depth!");
	}

	void DirectX11::CreateDisabledDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC  stencilDesc;
		ZeroMemory(&stencilDesc, sizeof(stencilDesc));

		stencilDesc.DepthEnable = false;
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

		HRESULT hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStates[u16(eDepthStencil::Z_DISABLED)]);
		SetDebugName(myDepthStates[u16(eDepthStencil::Z_DISABLED)], "eDepthStencil::Z_DISABLED");
		HandleErrors(hr, "Failed to setup depth buffer!");
	}

	void DirectX11::CreateReadDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC  stencilDesc;
		ZeroMemory(&stencilDesc, sizeof(stencilDesc));


		stencilDesc.DepthEnable = TRUE;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;

		stencilDesc.StencilEnable = TRUE;
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

		HRESULT hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStates[u16(eDepthStencil::READ_NO_WRITE)]);
		SetDebugName(myDepthStates[u16(eDepthStencil::READ_NO_WRITE)], "eDepthStencil::READ_NO_WRITE");
		HandleErrors(hr, "Failed to setup depth buffer!");
	}

	void DirectX11::CreateDepthStencilStates()
	{
		CreateEnabledDepthStencilState();
		CreateDisabledDepthStencilState();
		CreateReadDepthStencilState();

		D3D11_DEPTH_STENCIL_DESC  stencilDesc;
		ZeroMemory(&stencilDesc, sizeof(stencilDesc));

		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		stencilDesc.StencilEnable = true;
		stencilDesc.StencilReadMask = 0xFF;
		stencilDesc.StencilWriteMask = 0xFF;

		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;

		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		HRESULT hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStates[u16(eDepthStencil::MASK_TEST)]);
		SetDebugName(myDepthStates[u16(eDepthStencil::MASK_TEST)], "eDepthStencil::MASK_TEST");
		HandleErrors(hr, "Failed to setup depth buffer!");


		ZeroMemory(&stencilDesc, sizeof(stencilDesc));
		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		stencilDesc.StencilEnable = false;
		stencilDesc.StencilReadMask = u8(0xFF);
		stencilDesc.StencilWriteMask = 0x0;

		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

		hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStates[u16(eDepthStencil::LIGHT_MASK)]);
		SetDebugName(myDepthStates[u16(eDepthStencil::LIGHT_MASK)], "eDepthStencil::LIGHT_MASK");
		HandleErrors(hr, "Failed to setup depth buffer!");






		//stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		//stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;

		//stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		//stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		//stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
		//stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		//stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		//stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;


		ZeroMemory(&stencilDesc, sizeof(stencilDesc));
		
		stencilDesc.DepthEnable = true;
		stencilDesc.StencilEnable = true;
		stencilDesc.StencilReadMask = 0xFF;
		stencilDesc.StencilWriteMask = 0xFF;

		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;

		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;

		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;


		hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStates[u16(eDepthStencil::DEPTH_TEST)]);
		SetDebugName(myDepthStates[u16(eDepthStencil::DEPTH_TEST)], "eDepthStencil::DEPTH_TEST");
		HandleErrors(hr, "Failed to setup depth buffer!");

	}
};
#endif