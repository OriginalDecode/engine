#include "stdafx.h"
#include "ShadowSpotlight.h"
#include <Utilities.h>

bool ShadowSpotlight::Initiate(const CU::Vector3f& position, const CU::Vector3f& direction, float buffer_size)
{
	m_BufferSize = buffer_size;
	m_Viewport = Engine::GetAPI()->CreateViewport(m_BufferSize, m_BufferSize, 0.f, 1.f, 0, 0);

	m_Camera = new Camera;
	m_Camera->CreatePerspectiveProjection(m_BufferSize, m_BufferSize, 1.f, 256.f, 90.f);
	m_Camera->SetIsShadowCamera(true);

	m_Camera->SetPosition(position);

	m_Camera->RotateAroundY(cl::DegreeToRad(90.f) * direction.x);
	m_Camera->RotateAroundZ(cl::DegreeToRad(90.f) * direction.y);
	m_Camera->RotateAroundX(cl::DegreeToRad(90.f) * direction.z);


	TextureDesc desc;
	desc.m_Usage = graphics::DEFAULT_USAGE;
	desc.m_ResourceTypeBinding = graphics::BIND_SHADER_RESOURCE | graphics::BIND_RENDER_TARGET;
	desc.m_TextureFormat = graphics::RGBA16_FLOAT;
	desc.m_RenderTargetFormat = graphics::RGBA16_FLOAT;
	desc.m_Width = m_BufferSize;
	desc.m_Height = m_BufferSize;


	m_Depth = new Texture;
	m_Depth->Initiate(desc, "ShadowSpotlight : Depth ");


	desc.m_ResourceTypeBinding = graphics::BIND_SHADER_RESOURCE | graphics::BIND_DEPTH_STENCIL;
	desc.m_TextureFormat = graphics::R32_TYPELESS;
	desc.m_RenderTargetFormat = graphics::R32_FLOAT;
	desc.m_DepthTextureFormat = graphics::DEPTH_32_FLOAT;

	m_DepthStencil = new Texture;
	m_DepthStencil->Initiate(desc, "ShadowSpotlight : DepthStencil ");

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

	SAFE_DELETE(m_Depth);

	SAFE_DELETE(m_DepthStencil);

	return true;
}

void ShadowSpotlight::SetViewport()
{
	Engine::GetAPI()->GetContext().SetViewport(m_Viewport);
}

void ShadowSpotlight::ClearTexture()
{
	graphics::IGraphicsContext& ctx = Engine::GetAPI()->GetContext();
	ctx.ClearRenderTarget(m_Depth->GetRenderTargetView(), clearcolor::black);
	ctx.ClearDepthStencilView(m_DepthStencil->GetDepthView(), graphics::DEPTH | graphics::STENCIL, 1);
}

void ShadowSpotlight::SetTargets()
{
	Engine::GetAPI()->GetContext().OMSetRenderTargets(1, m_DepthStencil->GetRenderTargetRef(), m_DepthStencil->GetDepthView());
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
