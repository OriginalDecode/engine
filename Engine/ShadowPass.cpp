#include "stdafx.h"
#include "ShadowPass.h"


bool ShadowPass::Initiate()
{

	return true;
}

bool ShadowPass::CleanUp()
{
	m_DepthTexture->CleanUp();
	SAFE_DELETE(m_DepthTexture);

	return true;
}

void ShadowPass::ProcessShadows(Camera* camera, const RenderContext& render_context)
{
	
	//render_context.m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	//Camera* old_camera = m_Camera;
	//m_Camera = camera;
	//m_Shadowlight->SetViewport();

	/*float clear[4] = { 0.f, 0.f, 0.f, 0.f };
	render_context.m_Context->ClearRenderTargetView(m_ShadowDepth->GetRenderTargetView(), clear);
	render_context.m_Context->ClearDepthStencilView(m_ShadowDepthStencil->GetDepthView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	render_context.m_Context->OMSetRenderTargets(1, m_ShadowDepth->GetRenderTargetRef(), m_ShadowDepthStencil->GetDepthView());

	render_context.m_API->SetVertexShader(m_ShadowEffect->GetVertexShader()->m_Shader);
	render_context.m_API->SetPixelShader(m_ShadowEffect->GetPixelShader()->m_Shader);
*/
	//m_ShadowEffect->Activate();

	//m_ProcessDirectionalShadows = true;

	for (std::function<void()> function : m_FuncPtrs)
	{
		function();
	}
	//Render3DCommands();
	//RenderParticles();

	//m_ProcessDirectionalShadows = false;

	//render_context.m_Engine->ResetRenderTargetAndDepth();
	//render_context.m_API->ResetViewport();

	//m_DirectionalFrame = m_Camera->GetOrientation();
	////m_Camera = old_camera;

	//m_ShadowEffect->Deactivate();

	//myDeferredRenderer->SetBuffers();






}

void ShadowPass::RegisterFunction(std::function<void()> pFunction)
{
	m_FuncPtrs.Add(pFunction);
}

