#include "stdafx.h"
#include "Shadow_Directional.h"
#include <Engine/Effect.h>
#include <Engine/Engine.h>

void ShadowDirectional::Initiate(float buffer_size)
{
	m_Camera = new Camera;
	m_Camera->CreateOrthographicProjection(512, 512, 0.1f, 1000.f);

	//m_Camera->SetPosition({ 55, 90, 55});
	m_Camera->RotateAroundX(cl::DegreeToRad(90.f) * 1.f);

	m_ShadowDepth = new Texture;
	m_ShadowDepth->InitiateAsRenderTarget(buffer_size, buffer_size, "DirectionalShadows : RTV");

	m_ShadowDepthStencil = new Texture;
	m_ShadowDepthStencil->InitiateAsDepthStencil(buffer_size, buffer_size, "DirectionalShadows : DSV");


	m_Viewport = Engine::GetAPI()->CreateViewport(buffer_size, buffer_size, 0.f, 1.f, 0, 0);

	m_ConstBuffer.RegisterVariable(&m_Camera->GetViewProjection());
	m_ConstBuffer.Initiate("directional_shadow");

	Engine::GetInstance()->GetEffect("Shaders/deferred_ambient.json")->AddShaderResource(m_ShadowDepthStencil->GetShaderView(), Effect::SHADOWMAP);


#if !defined(_PROFILE) && !defined(_FINAL)
	debug::DebugHandle::GetInstance()->RegisterTexture(m_ShadowDepth, "Directional Shadow");
	debug::DebugHandle::GetInstance()->RegisterTexture(m_ShadowDepthStencil, "Directional Shadow Stencil");
#endif

}

void ShadowDirectional::CleanUp()
{
	SAFE_DELETE(m_Viewport);
	SAFE_DELETE(m_Camera);
	SAFE_DELETE(m_ShadowDepth);
	SAFE_DELETE(m_ShadowDepthStencil);
}

void ShadowDirectional::SetViewport()
{
	Engine::GetAPI()->GetContext().SetViewport(m_Viewport);
}

void ShadowDirectional::ClearTexture()
{
	graphics::IGraphicsContext& ctx = Engine::GetAPI()->GetContext();
	ctx.ClearRenderTarget(m_ShadowDepth->GetRenderTargetView(), clearcolor::black);
	ctx.ClearDepthStencilView(m_ShadowDepthStencil->GetDepthView(), graphics::DEPTH | graphics::STENCIL, 1.0f);
}

void ShadowDirectional::SetTargets()
{
	Engine::GetAPI()->GetContext().OMSetRenderTargets(1, m_ShadowDepth->GetRenderTargetRef(), m_ShadowDepthStencil->GetDepthView());
}

void ShadowDirectional::SetOrientation(const CU::Matrix44f& orientation)
{
	m_Camera->SetOrientation(orientation); 
}

void ShadowDirectional::Update()
{
#ifdef _DEBUG
	debug::DebugHandle* pDebug = debug::DebugHandle::GetInstance();
	m_Camera->SetPosition(pDebug->m_CamPos);
	m_Camera->SetRotationX(cl::DegreeToRad(pDebug->m_CamRot[0]));
	m_Camera->SetRotationY(cl::DegreeToRad(pDebug->m_CamRot[1]));
	m_Camera->SetRotationZ(cl::DegreeToRad(pDebug->m_CamRot[2]));
#endif
	m_Camera->Update();
}

CU::Matrix44f ShadowDirectional::GetMVP()
{
	const CU::Matrix44f& orientation = CU::Math::Inverse(m_Camera->GetCurrentOrientation());
	const CU::Matrix44f& perspective = m_Camera->GetOrthographic();
	return orientation * perspective;
}
