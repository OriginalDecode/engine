#include "stdafx.h"
#include "ShadowPass.h"
#include <Engine/Renderer.h>

#include <Engine/Shadow_Directional.h>
#include <Engine/ShadowSpotlight.h>

bool ShadowPass::Initiate(Renderer* renderer)
{
	m_RenderToDepthTechnique = Engine::GetInstance()->GetEffect("Shaders/T_Render_Depth.json");
	return true;
}

bool ShadowPass::CleanUp()
{
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
	shadow_spotlight->ClearTexture();
	shadow_spotlight->SetTargets();
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

