#include "stdafx.h"
#include "ShadowPass.h"
#include <Engine/Renderer.h>

#include <Engine/Shadow_Directional.h>
#include <Engine/ShadowSpotlight.h>

bool ShadowPass::Initiate(Renderer* renderer)
{
	m_RenderToDepth = Engine::GetInstance()->GetEffect("Shaders/render_depth.json");
	m_Renderer = renderer;
	return true;
}

bool ShadowPass::CleanUp()
{
	return true;
}

void ShadowPass::ProcessShadows(Camera* camera)
{
	//render_context.m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_Renderer->Render3DShadows(camera->GetOrientation(), camera);
}

void ShadowPass::ProcessShadows(ShadowSpotlight* shadow_spotlight)
{
	shadow_spotlight->SetViewport();
	shadow_spotlight->ClearTexture();
	shadow_spotlight->SetTargets();
	m_RenderToDepth->Use();
	ProcessShadows(shadow_spotlight->GetCamera());
	m_RenderToDepth->Clear();
	//Engine::GetAPI()->ResetViewport();
}

void ShadowPass::ProcessShadows(ShadowDirectional* shadow_directional)
{
	PROFILE_FUNCTION(profiler::colors::DarkBlue);
	shadow_directional->SetViewport();
	shadow_directional->ClearTexture();
	shadow_directional->SetTargets();
	m_RenderToDepth->Use();
	ProcessShadows(shadow_directional->GetCamera());
	m_RenderToDepth->Clear();
	//Engine::GetAPI()->ResetViewport();
}

void ShadowPass::Activate()
{
	m_RenderToDepth->Use();
}

void ShadowPass::DeActivate()
{
	m_RenderToDepth->Clear();
}

const CU::Matrix44f& ShadowPass::GetOrientation()
{
	return m_Orientation;
}

const CU::Matrix44f& ShadowPass::GetOrientation() const
{
	return m_Orientation;
}

