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

	m_DebugTexture = new Texture;
	m_DebugTexture->Initiate(
		m_WindowSize.m_Width,
		m_WindowSize.m_Height,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		"SSAOPass | SSAOTexture2");

	m_Test = new Texture;
	m_Test->Initiate(
		m_WindowSize.m_Width,
		m_WindowSize.m_Height,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		"SSAOPass | SSAOTexture3");



	m_ScreenQuad.Initiate();
	m_SSAOShader = Engine::GetInstance()->GetEffect("Shaders/T_SSAO.json");

	m_cbSSAO = Engine::GetAPI()->CreateConstantBuffer(sizeof(cbSSAO));
	m_cbBlur = Engine::GetAPI()->CreateConstantBuffer(sizeof(cbBlur));
	
// 	m_BlurShader[0] = Engine::GetInstance()->GetEffect("Shaders/T_Blur_x.json");
// 	m_BlurShader[0]->AddShaderResource(m_SSAOTexture, Effect::DIFFUSE);
// 
// 	m_BlurShader[1] = Engine::GetInstance()->GetEffect("Shaders/T_Blur_y.json");
// 	m_BlurShader[1]->AddShaderResource(m_DebugTexture, Effect::DIFFUSE);
#if !defined(_PROFILE) && !defined(_FINAL)
	Engine::GetInstance()->AddTexture(m_SSAOTexture, "SSAO");
#endif
	Engine::GetInstance()->GetEffect("Shaders/T_Deferred_Ambient.json")->AddShaderResource(m_SSAOTexture, Effect::SSAO);
}

void SSAOPass::CleanUp()
{
	m_ScreenQuad.CleanUp();

	SAFE_RELEASE(m_cbBlur);
	SAFE_RELEASE(m_cbSSAO);

	m_SSAOTexture->CleanUp();
	SAFE_DELETE(m_SSAOTexture);

	m_DebugTexture->CleanUp();
	SAFE_DELETE(m_DebugTexture);

	m_Test->CleanUp();
	SAFE_DELETE(m_Test);
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
	ctx->ClearRenderTargetView(m_DebugTexture->GetRenderTargetView(), clear);
	//ctx->ClearRenderTargetView(m_Test->GetRenderTargetView(), clear);

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


	/*m_BlurStruct.m_Size = 1.f / m_WindowSize.m_Width * 0.25f;
	api->UpdateConstantBuffer(m_cbBlur, &m_BlurStruct);


	ctx->PSSetConstantBuffers(0, 1, &m_cbBlur);
	ctx->OMSetRenderTargets(1, m_Test->GetRenderTargetRef(), api->GetDepthView());

	m_BlurShader[0]->Use();
	m_ScreenQuad.Render();
	m_BlurShader[0]->Clear();

	m_BlurStruct.m_Size = 1.f / m_WindowSize.m_Height * 0.25f;
	api->UpdateConstantBuffer(m_cbBlur, &m_BlurStruct);

	m_BlurShader[1]->Use();
	m_ScreenQuad.Render();
	m_BlurShader[1]->Clear();*/


}
