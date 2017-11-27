#include "stdafx.h"
#include "DirectX11.h"
#include <Engine/DX11Device.h>
namespace graphics
{
	void DirectX11::CreateBlendStates()
	{
		ID3D11Device* device = static_cast<DX11Device*>(m_Device)->m_Device;


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
		ID3D11BlendState* blend_state = nullptr;
		device->CreateBlendState(&blendDesc, &blend_state);
		m_BlendStates[NO_BLEND] = blend_state;
#ifndef FINAL
		SetDebugName(blend_state, "NO_BLEND BlendState");
#endif
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE; // dest blend makes it go bonkers?
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE; //no difference between zero & one
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;

		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;
		blend_state = nullptr;
		device->CreateBlendState(&blendDesc, &blend_state);
		m_BlendStates[LIGHT_BLEND] = blend_state;
#ifndef FINAL
		SetDebugName(blend_state, "LIGHT_BLEND BlendState");
#endif
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
		blend_state = nullptr;
		device->CreateBlendState(&blendDesc, &blend_state);
		m_BlendStates[ALPHA_BLEND] = blend_state;
#ifndef FINAL
		SetDebugName(blend_state, "ALPHA_BLEND BlendState");
#endif
		blendDesc.RenderTarget[0].BlendEnable = FALSE;
		blend_state = nullptr;
		device->CreateBlendState(&blendDesc, &blend_state);
		m_BlendStates[BLEND_FALSE] = blend_state;
#ifndef FINAL
		SetDebugName(blend_state, "BLEND_FALSE BlendState");
#endif
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;

		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;

		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;
		device->CreateBlendState(&blendDesc, &blend_state);
		m_BlendStates[PARTICLE_BLEND] = blend_state;
#ifndef FINAL
		SetDebugName(blend_state, "PARTICLE_BLEND BlendState");
#endif
	}
};