#include "stdafx.h"
#include "ShadowPass.h"
#include <Engine/Renderer.h>

bool ShadowPass::Initiate(Renderer* renderer)
{
	m_ShadowSpotlight = new ShadowSpotlight;

	m_ShadowSpotlight->Initiate(
		CU::Vector3f(256.f, 128.f, 256.f) //Position
		, CU::Vector3f(0.f, 0.f, 1.f) //Direction
		, 2048.f); // Buffer Size

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

	/*
	
		This will have to check if the entity is within range as well.
		Or maybe the Render3DCommands function can do this instead?
		This function should probably be responsible for this. 
		They might use camerahandles instead that bundles the camera with a frustum & 
		bounding box.
	
	*/


	render_context.m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	Camera* old_camera = m_Camera;
	m_Camera = camera;
	m_Shadowlight->SetViewport();

	float clear[4] = { 0.f, 0.f, 0.f, 0.f };
	render_context.m_Context->ClearRenderTargetView(m_ShadowDepth->GetRenderTargetView(), clear);
	render_context.m_Context->ClearDepthStencilView(m_ShadowDepthStencil->GetDepthView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	render_context.m_Context->OMSetRenderTargets(1, m_ShadowDepth->GetRenderTargetRef(), m_ShadowDepthStencil->GetDepthView());

	render_context.m_API->SetVertexShader(m_RenderToDepthTechnique->GetVertexShader()->m_Shader);
	render_context.m_API->SetPixelShader(m_RenderToDepthTechnique->GetPixelShader()->m_Shader);

	m_RenderToDepthTechnique->Activate();

	m_ProcessDirectionalShadows = true;

	
	m_Renderer->Render3DShadows(camera->GetOrientation(), camera);


	Render3DCommands();
	RenderParticles();

	m_ProcessDirectionalShadows = false;

	render_context.m_Engine->ResetRenderTargetAndDepth();
	render_context.m_API->ResetViewport();

	m_DirectionalFrame = m_Camera->GetOrientation();
	//m_Camera = old_camera;

	m_RenderToDepthTechnique->Deactivate();

	myDeferredRenderer->SetBuffers();

}

const CU::Matrix44f& ShadowPass::GetOrientation()
{
	return m_Orientation;
}

const CU::Matrix44f& ShadowPass::GetOrientation() const
{
	return m_Orientation;
}

