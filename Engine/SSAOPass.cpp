#include "stdafx.h"
#include "SSAOPass.h"

void SSAOPass::Initiate()
{
	m_WindowSize = Engine::GetInstance()->GetInnerSize();
	m_SSAOTexture = new Texture;
	m_SSAOTexture->Initiate(
		m_WindowSize.m_Width,
		m_WindowSize.m_Height,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		"SSAOPass | SSAOTexture");

	m_ScreenQuad.Initiate();
	m_SSAOShader = Engine::GetInstance()->GetEffect("Shaders/T_SSAO.json");


	Engine::GetInstance()->GetEffect("Shaders/T_Deferred_Ambient.json")->AddShaderResource(m_SSAOTexture, Effect::SSAO);
}

void SSAOPass::CleanUp()
{
	SAFE_DELETE(m_SSAOTexture);
}

void SSAOPass::Process()
{
	m_ScreenQuad.SetBuffers();

	IDevContext* ctx = Engine::GetAPI()->GetContext();
	float clear[4] = { 0.f, 0.f, 0.f, 0.f };
	ctx->ClearRenderTargetView(m_SSAOTexture->GetRenderTargetView(), clear);
	ctx->OMSetRenderTargets(1, m_SSAOTexture->GetRenderTargetRef(), nullptr);
	
	m_SSAOShader->Use();
	m_ScreenQuad.Render();
	m_SSAOShader->Clear();
}
