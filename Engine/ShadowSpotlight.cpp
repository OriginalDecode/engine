#include "stdafx.h"
#include "ShadowSpotlight.h"
#include <Utilities.h>
namespace Snowblind
{
	bool ShadowSpotlight::Initiate(const CU::Vector3f& position, const CU::Vector3f& direction, float buffer_size)
	{
		m_BufferSize = buffer_size;
		m_Device = Engine::GetInstance()->GetAPI()->GetDevice();
		m_Context = Engine::GetInstance()->GetAPI()->GetContext();
		m_Viewport = Engine::GetInstance()->GetAPI()->CreateViewport(1920, 1080, 0.f, 1.f, 0, 0);

		m_Camera = new Camera(1920, 1080, 50.f, 1.f, 90.f);
		m_Camera->SetPosition(position);
		m_Camera->RotateAroundY(CL::DegreeToRad(90.f) * direction.x);
		m_Camera->RotateAroundZ(CL::DegreeToRad(90.f) * direction.z);
		m_Camera->RotateAroundX(CL::DegreeToRad(90.f) * direction.y);


		m_Depth = new Texture;
		m_Depth->Initiate(1920, 1080,
			DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R16G16B16A16_FLOAT
			, "Shadowlight : Depth ");

		m_Holder = new Texture;
		m_Holder->Initiate(1920, 1080
			, DEFAULT_USAGE | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL
			, DXGI_FORMAT_R32_TYPELESS
			, DXGI_FORMAT_R32_FLOAT
			, DXGI_FORMAT_D32_FLOAT
			, "Shadowlight : DepthStencil ");

		m_DepthStencil = new Texture;
		m_DepthStencil->Initiate(1920, 1080
			, DEFAULT_USAGE | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL
			, DXGI_FORMAT_R32_TYPELESS
			, DXGI_FORMAT_R32_FLOAT
			, DXGI_FORMAT_D32_FLOAT
			, "Shadowlight : DepthStencil ");

		m_ShadowEffect = Engine::GetInstance()->GetEffect("Data/Shaders/T_Render_Depth.json");
		m_ShadowEffect->AddShaderResource(m_Holder->GetDepthStencilView());
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

		m_Depth->CleanUp();
		SAFE_DELETE(m_Depth);
		if (m_Depth)
			return false;

		m_DepthStencil->CleanUp();
		SAFE_DELETE(m_DepthStencil);
		if (m_DepthStencil)
			return false;

		m_Holder->CleanUp();
		SAFE_DELETE(m_Holder);
		if (m_Holder)
			return false;

		return true;
	}

	void ShadowSpotlight::SetViewport()
	{
		Engine::GetInstance()->GetAPI()->SetViewport(m_Viewport);
	}

	void ShadowSpotlight::ClearTexture()
	{
		float clear[4] = { 0.f, 0.f, 0.f, 0.f };
		m_Context->ClearRenderTargetView(m_Depth->GetRenderTargetView(), clear);
		m_Context->ClearDepthStencilView(m_DepthStencil->GetDepthView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void ShadowSpotlight::SetTargets()
	{
		ID3D11RenderTargetView* target = m_Depth->GetRenderTargetView();
		m_Context->OMSetRenderTargets(1, &target, m_DepthStencil->GetDepthView());

		DirectX11* api = Engine::GetInstance()->GetAPI();
		api->SetVertexShader(m_ShadowEffect->GetVertexShader()->m_Shader);
		api->SetPixelShader(m_ShadowEffect->GetPixelShader()->m_Shader);
	}

	void ShadowSpotlight::ToggleShader(bool on_or_off)
	{
		if (on_or_off)
			m_ShadowEffect->Activate();
		else
			m_ShadowEffect->Deactivate();
	}

	CU::Matrix44f ShadowSpotlight::GetOrientation()
	{
		return m_Camera->GetOrientation();
	}

	CU::Matrix44f ShadowSpotlight::GetMVP()
	{
		return CU::Matrix44f::CreateScaleMatrix(CU::Vector4f(5,5,5,1) * CU::Math::Inverse(m_Camera->GetOrientation()) * m_Camera->GetProjection());
	}

	void ShadowSpotlight::Copy()
	{
		Texture::CopyData(m_Holder->GetDepthTexture(), m_DepthStencil->GetDepthTexture());
	}

};