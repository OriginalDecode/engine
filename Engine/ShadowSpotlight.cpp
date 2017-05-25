#include "stdafx.h"
#include "ShadowSpotlight.h"
#include <Utilities.h>

bool ShadowSpotlight::Initiate(const CU::Vector3f& position, const CU::Vector3f& direction, float buffer_size)
{
	m_BufferSize = buffer_size;
	m_Context = Engine::GetInstance()->GetAPI()->GetContext();
	m_Viewport = Engine::GetInstance()->GetAPI()->CreateViewport(m_BufferSize, m_BufferSize, 0.f, 1.f, 0, 0);

	m_Camera = new Camera;
	m_Camera->CreatePerspectiveProjection(m_BufferSize, m_BufferSize, 1.f, 256.f, 90.f);
	m_Camera->SetIsShadowCamera(true);

	m_Camera->SetPosition(position);

	m_Camera->RotateAroundY(CL::DegreeToRad(90.f) * direction.x);
	m_Camera->RotateAroundZ(CL::DegreeToRad(90.f) * direction.y);
	m_Camera->RotateAroundX(CL::DegreeToRad(90.f) * direction.z);

	m_Depth = new Texture;
	m_Depth->Initiate(m_BufferSize, m_BufferSize,
		DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R16G16B16A16_FLOAT
		, "ShadowSpotlight : Depth ");

#ifdef _DEBUG
	m_Holder = new Texture;
	m_Holder->Initiate(m_BufferSize, m_BufferSize
		, DEFAULT_USAGE | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL
		, DXGI_FORMAT_R32_TYPELESS
		, DXGI_FORMAT_R32_FLOAT
		, DXGI_FORMAT_D32_FLOAT
		, "ShadowSpotlight : DepthStencil ");
#endif
	m_DepthStencil = new Texture;
	m_DepthStencil->Initiate(m_BufferSize, m_BufferSize
		, DEFAULT_USAGE | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL
		, DXGI_FORMAT_R32_TYPELESS
		, DXGI_FORMAT_R32_FLOAT
		, DXGI_FORMAT_D32_FLOAT
		, "ShadowSpotlight : DepthStencil ");

	return true;
}

void ShadowSpotlight::Initiate(float buffer_size)
{
	Initiate(CU::Vector3f(0.f, 0.f, 0.f), CU::Vector3f(0.f, 0.f, 0.f), buffer_size);
}

bool ShadowSpotlight::CleanUp()
{
	SAFE_DELETE(m_Viewport);
	SAFE_DELETE(m_Camera);

	m_Depth->CleanUp();
	SAFE_DELETE(m_Depth);

	m_DepthStencil->CleanUp();
	SAFE_DELETE(m_DepthStencil);

#ifdef _DEBUG
	m_Holder->CleanUp();
	SAFE_DELETE(m_Holder);
#endif
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
}

void ShadowSpotlight::SetOrientation(const CU::Matrix44f& orientation)
{
	m_Camera->SetOrientation(orientation);
}

void ShadowSpotlight::SetAngle(float angle)
{
	m_Camera->SetFOV(angle);
}

const CU::Matrix44f& ShadowSpotlight::GetOrientation()
{
	return m_Camera->GetOrientation();
}

const CU::Matrix44f& ShadowSpotlight::GetMVP()
{
	return (CU::Math::Inverse(m_Camera->GetOrientation()) * m_Camera->GetPerspective());
}

#ifdef _DEBUG
void ShadowSpotlight::Copy()
{
	Texture::CopyData(m_Holder->GetTexture(), m_DepthStencil->GetDepthTexture());
}
#endif

