#include "stdafx.h"
#include "ShadowSpotlight.h"
namespace Snowblind
{
	bool ShadowSpotlight::Initiate(const CU::Vector3f& position, const CU::Vector3f& direction)
	{
		m_Viewport = CEngine::GetInstance()->GetAPI()->CreateViewport(2048, 2048, 0.f, 1.f, 0, 0);
		m_Camera = new CCamera(2048.f, 2048.f);

		m_Device = CEngine::GetInstance()->GetAPI()->GetDevice();
		m_Context = CEngine::GetInstance()->GetAPI()->GetContext();

		m_Texture = new CTexture();
		m_Texture->InitStencil(2048.f, 2048.f);

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