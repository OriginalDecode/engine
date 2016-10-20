#include "stdafx.h"
#include "DirectX11.h"
#ifdef SNOWBLIND_DX11
namespace Snowblind
{
	void DirectX11::CreateBlendStates()
	{
		D3D11_BLEND_DESC blendDesc;
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;

		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;
		myDevice->CreateBlendState(&blendDesc, &myBlendStates[u16(eBlendStates::ALPHA_BLEND)]);
		SetDebugName(myBlendStates[u16(eBlendStates::ALPHA_BLEND)], "ALPHA_BLEND BlendState");

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE; // dest blend makes it go bonkers?
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE; //no difference between zero & one
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;
		myDevice->CreateBlendState(&blendDesc, &myBlendStates[u16(eBlendStates::LIGHT_BLEND)]);
		SetDebugName(myBlendStates[u16(eBlendStates::LIGHT_BLEND)], "LIGHT_BLEND BlendState");

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;
		myDevice->CreateBlendState(&blendDesc, &myBlendStates[u16(eBlendStates::PARTICLE_BLEND)]);
		SetDebugName(myBlendStates[u16(eBlendStates::PARTICLE_BLEND)], "PARTICLE_BLEND BlendState");


		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		myDevice->CreateBlendState(&blendDesc, &myBlendStates[u16(eBlendStates::NO_BLEND)]);
		SetDebugName(myBlendStates[u16(eBlendStates::NO_BLEND)], "NO_BLEND BlendState");

		blendDesc.RenderTarget[0].BlendEnable = FALSE;
		myDevice->CreateBlendState(&blendDesc, &myBlendStates[u16(eBlendStates::BLEND_FALSE)]);
		SetDebugName(myBlendStates[u16(eBlendStates::BLEND_FALSE)], "NO_BLEND BlendState");


	}
};
#endif