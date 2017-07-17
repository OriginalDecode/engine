#include "stdafx.h"
#include "Shadow_Directional.h"
#include <Engine/Effect.h>
#include <Engine/Engine.h>
void ShadowDirectional::Initiate(float buffer_size)
{
	m_Camera = new Camera;
	m_Camera->CreateOrthographicProjection(200.f, 200.f, 1.f, 1024.f);

	m_Camera->SetPosition({ 1024.f, 512.f, 512.f });
	m_Camera->RotateAroundX(CL::DegreeToRad(90.f) * 1.f);

	m_ShadowDepth = new Texture;
	m_ShadowDepth->InitiateAsRenderTarget(buffer_size, buffer_size, "DirectionalShadows : RTV");

	m_ShadowDepthStencil = new Texture;
	m_ShadowDepthStencil->InitiateAsDepthStencil(buffer_size, buffer_size, "DirectionalShadows : DSV");


	m_Viewport = Engine::GetAPI()->CreateViewport(buffer_size, buffer_size, 0.f, 1.f, 0, 0);


	Engine::GetInstance()->GetEffect("Shaders/deferred_ambient.json")->AddShaderResource(m_ShadowDepthStencil->GetShaderView(), Effect::SHADOWMAP);

	Engine::GetInstance()->AddTexture(m_ShadowDepth, "Directional Shadow");
	Engine::GetInstance()->AddTexture(m_ShadowDepthStencil, "Directional Shadow Stencil");


}

void ShadowDirectional::CleanUp()
{
	SAFE_DELETE(m_Viewport);
	SAFE_DELETE(m_Camera);

	m_ShadowDepth->CleanUp();
	SAFE_DELETE(m_ShadowDepth);

	m_ShadowDepthStencil->CleanUp();
	SAFE_DELETE(m_ShadowDepthStencil);
}

void ShadowDirectional::SetViewport()
{
	Engine::GetAPI()->SetViewport(m_Viewport);
}

void ShadowDirectional::ClearTexture(const RenderContext& render_context)
{
	render_context.m_Context->ClearRenderTargetView(m_ShadowDepth->GetRenderTargetView(), m_Clear);
	render_context.m_Context->ClearDepthStencilView(m_ShadowDepthStencil->GetDepthView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void ShadowDirectional::SetTargets(const RenderContext& render_context)
{
	render_context.m_Context->OMSetRenderTargets(1, m_ShadowDepth->GetRenderTargetRef(), m_ShadowDepthStencil->GetDepthView());
}

void ShadowDirectional::SetOrientation(const CU::Matrix44f& orientation)
{
	m_Camera->SetOrientation(orientation); 
}

const CU::Matrix44f& ShadowDirectional::GetMVP()
{
	return CU::Math::Inverse(m_Camera->GetOrientation()) * m_Camera->GetPerspective(); 
}
