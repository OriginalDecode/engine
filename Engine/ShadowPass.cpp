#include "stdafx.h"
#include "ShadowPass.h"
#include <Engine/Renderer.h>

#include <Engine/Shadow_Directional.h>
#include <Engine/ShadowSpotlight.h>

bool ShadowPass::Initiate(Renderer* renderer)
{
	m_ShadowSpotlight = new ShadowSpotlight;

	//m_ShadowSpotlight->Initiate(
	//	CU::Vector3f(256.f, 128.f, 256.f) //Position
	//	, CU::Vector3f(0.f, 0.f, 1.f) //Direction
	//	, 2048.f); // Buffer Size

	m_RenderToDepthTechnique = Engine::GetInstance()->GetEffect("Shaders/T_Render_Depth.json");
	
	return true;
}

bool ShadowPass::CleanUp()
{
	m_DepthTexture->CleanUp();
	SAFE_DELETE(m_DepthTexture);

	m_ShadowSpotlight->CleanUp();
	SAFE_DELETE(m_ShadowSpotlight);
	return true;
}

void ShadowPass::ProcessShadows(Camera* camera, const RenderContext& render_context)
{
	render_context.m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_Renderer->Render3DShadows(camera->GetOrientation(), camera);
	myDeferredRenderer->SetBuffers();

}

void ShadowPass::ProcessShadows(ShadowSpotlight* shadow_spotlight, const RenderContext& render_context)
{
	shadow_spotlight->SetViewport();
	ProcessShadows(shadow_spotlight->GetCamera(), render_context);
}

void ShadowPass::ProcessShadows(ShadowDirectional* shadow_directional, const RenderContext& render_context)
{
	shadow_directional->SetViewport();
	ProcessShadows(shadow_directional->GetCamera(), render_context);
}

void ShadowPass::Activate()
{
	m_RenderToDepthTechnique->Activate();
}

void ShadowPass::DeActivate()
{
	m_RenderToDepthTechnique->Deactivate();
}

const CU::Matrix44f& ShadowPass::GetOrientation()
{
	return m_Orientation;
}

const CU::Matrix44f& ShadowPass::GetOrientation() const
{
	return m_Orientation;
}

