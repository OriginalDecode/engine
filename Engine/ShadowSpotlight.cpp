#include "stdafx.h"
#include "ShadowSpotlight.h"
namespace Snowblind
{
	bool ShadowSpotlight::Initiate(const CU::Vector3f& /*position*/, const CU::Vector3f& /*direction*/)
	{
		m_Viewport = CEngine::GetInstance()->GetAPI()->CreateViewport(2048, 2048, 0.f, 1.f, 0, 0);
		m_Camera = new CCamera(2048.f, 2048.f);

		m_Device = CEngine::GetInstance()->GetAPI()->GetDevice();
		m_Context = CEngine::GetInstance()->GetAPI()->GetContext();

		//m_Texture = new CTexture;
		//m_Texture->InitStencil(2048.f, 2048.f);
		//myDepthTexture->Initiate(CEngine::GetInstance()->GetWindowSize().myWidth, CEngine::GetInstance()->GetWindowSize().myHeight, DEFAULT_USAGE | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL
		//, DXGI_FORMAT_R24G8_TYPELESS, DXGI_FORMAT_R24_UNORM_X8_TYPELESS, DXGI_FORMAT_D24_UNORM_S8_UINT, "Renderer : Depth");
		return true;
	}

	bool ShadowSpotlight::CleanUp()
	{
		SAFE_DELETE(m_Viewport);
		if (m_Viewport)
			return false;

		SAFE_DELETE(m_Camera);
		if (m_Camera)
			return false;

		SAFE_DELETE(m_Texture);
		if (m_Texture)
			return false;

		return true;
	}

	void ShadowSpotlight::SetViewport()
	{
		CEngine::GetInstance()->GetAPI()->SetViewport(m_Viewport);
	}

	void ShadowSpotlight::ClearTexture()
	{
		float clear[4] = { 0.f, 0.f, 0.f, 0.f };
		m_Context->ClearRenderTargetView(m_Texture->GetRenderTargetView(), clear);
	}

};