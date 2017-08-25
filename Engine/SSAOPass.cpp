#include "stdafx.h"
#include "SSAOPass.h"

#include <Engine/Engine.h>
#include <Engine/Synchronizer.h>

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
	m_SSAOShader = Engine::GetInstance()->GetEffect("Shaders/ssao.json");

	m_cbSSAO = Engine::GetAPI()->CreateConstantBuffer(sizeof(cbSSAO));
	m_cbBlur = Engine::GetAPI()->CreateConstantBuffer(sizeof(cbBlur));
	
#if !defined(_PROFILE) && !defined(_FINAL)
	//Engine::GetInstance()->AddTexture(m_SSAOTexture, "SSAO");
#endif
	Engine::GetInstance()->GetEffect("Shaders/deferred_ambient.json")->AddShaderResource(m_SSAOTexture, Effect::SSAO);
}

void SSAOPass::CleanUp()
{

	SAFE_RELEASE(m_cbBlur);
	SAFE_RELEASE(m_cbSSAO);

	SAFE_DELETE(m_SSAOTexture);


}

void SSAOPass::Process(Texture* scene_texture)
{
	Engine* engine = Engine::GetInstance();
	DirectX11* api = engine->GetAPI();


	m_SSAOStruct.m_Projection = CU::Math::InverseReal(engine->GetCamera()->GetPerspective());
	m_SSAOStruct.m_View = engine->GetCamera()->GetOrientation();
	


	api->UpdateConstantBuffer(m_cbSSAO, &m_SSAOStruct);


	m_ScreenQuad.SetBuffers();

	IDevContext* ctx = api->GetContext();
	float clear[4] = { 0.f, 0.f, 0.f, 0.f };
	ctx->ClearRenderTargetView(m_SSAOTexture->GetRenderTargetView(), clear);

	ID3D11RenderTargetView* rtv[] = {
		m_SSAOTexture->GetRenderTargetView(),
	};

	ctx->PSSetConstantBuffers(0, 1, &m_cbSSAO);
	ctx->OMSetRenderTargets(ARRAYSIZE(rtv), rtv, api->GetDepthView());

	api->SetSamplerState(eSamplerStates::LINEAR_WRAP);

	m_SSAOShader->Use();
	m_ScreenQuad.Render();
	m_SSAOShader->Clear();

	ID3D11RenderTargetView* rtv0[ARRAYSIZE(rtv)] = { };
	ctx->OMSetRenderTargets(ARRAYSIZE(rtv0), rtv0, api->GetDepthView());

}
