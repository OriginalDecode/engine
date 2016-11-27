#include "stdafx.h"
#include "ShadowSpotlight.h"
#include <Utilities.h>
namespace Snowblind
{
	bool ShadowSpotlight::Initiate(const CU::Vector3f& position, const CU::Vector3f& direction, float buffer_size)
	{
		m_BufferSize = buffer_size;
		m_Viewport = CEngine::GetInstance()->GetAPI()->CreateViewport(m_BufferSize, m_BufferSize, 0.f, 1.f, 0, 0);

		m_Camera = new CCamera(m_BufferSize, m_BufferSize, 25.f, 0.01f, 90.f);
		m_Camera->SetPosition(position);
		m_Camera->RotateAroundY(CL::DegreeToRad(90.f) * direction.x);
		m_Camera->RotateAroundX(CL::DegreeToRad(90.f) * direction.y);

		
		m_Device = CEngine::GetInstance()->GetAPI()->GetDevice();
		m_Context = CEngine::GetInstance()->GetAPI()->GetContext();

		m_RenderTarget = new CTexture;
		m_RenderTarget->Initiate(m_BufferSize, m_BufferSize, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R16G16B16A16_FLOAT, "Shadowlight : ");

		m_Normal = new CTexture;
		m_Normal->Initiate(m_BufferSize, m_BufferSize, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R16G16B16A16_FLOAT, "Shadowlight : ");

		m_Depth = new CTexture;
		m_Depth->Initiate(m_BufferSize, m_BufferSize, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R16G16B16A16_FLOAT, "Shadowlight : ");

		m_Texture = new CTexture;
		m_Texture->Initiate(m_BufferSize, m_BufferSize , DEFAULT_USAGE | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_RENDER_TARGET
			, DXGI_FORMAT_R16G16B16A16_FLOAT
			, DXGI_FORMAT_R32_TYPELESS
			, DXGI_FORMAT_R32_FLOAT
			, DXGI_FORMAT_D32_FLOAT
			, "Shadowlight : ");

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

		m_Texture->CleanUp();
		SAFE_DELETE(m_Texture);
		if (m_Texture)
			return false;

		m_RenderTarget->CleanUp();
		SAFE_DELETE(m_RenderTarget);
		if (m_RenderTarget)
			return false;

		m_Depth->CleanUp();
		SAFE_DELETE(m_Depth);
		if (m_Depth)
			return false;

		m_Normal->CleanUp();
		SAFE_DELETE(m_Normal);
		if (m_Normal)
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


		m_Context->ClearRenderTargetView(m_RenderTarget->GetRenderTargetView(), clear);
		m_Context->ClearDepthStencilView(m_Texture->GetDepthView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void ShadowSpotlight::SetTargets()
	{
		ID3D11RenderTargetView* target[3];
		target[0] = m_RenderTarget->GetRenderTargetView();
		target[1] = m_Normal->GetRenderTargetView();
		target[2] = m_Depth->GetRenderTargetView();

		m_Context->OMSetRenderTargets(3, target, m_Texture->GetDepthView());
	}

	CU::Matrix44f ShadowSpotlight::GetOrientation()
	{
		return m_Camera->GetOrientation();
	}

};