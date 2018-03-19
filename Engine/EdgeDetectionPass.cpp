#include "stdafx.h"
#include "EdgeDetectionPass.h"
#include <Engine/Texture.h>
#include <Engine/Quad.h>

EdgeDetectionPass::EdgeDetectionPass()
	: IPostprocessPass()
{
}


EdgeDetectionPass::~EdgeDetectionPass()
{
	Engine::GetAPI()->ReleasePtr(m_cbEdgeDetection);
	SAFE_DELETE(m_Result);

	delete m_Quad;
	m_Quad = nullptr;
}

void EdgeDetectionPass::Initiate()
{
	m_WindowSize = Engine::GetInstance()->GetInnerSize();

	TextureDesc desc;
	desc.m_ResourceTypeBinding = graphics::BIND_RENDER_TARGET | graphics::BIND_SHADER_RESOURCE;
	desc.m_Width = m_WindowSize.m_Width;
	desc.m_Height = m_WindowSize.m_Height;

	desc.m_TextureFormat = graphics::RGBA8_UNORM;
	desc.m_ShaderResourceFormat = graphics::RGBA8_UNORM;
	desc.m_RenderTargetFormat = graphics::RGBA8_UNORM;

	m_Result = new Texture;
	m_Result->Initiate(desc, false, "EdgeDetection - Result");

	//Effect* debug_textures = Engine::GetInstance()->GetEffect("Shaders/debug_textures.json");
#ifdef _DEBUG
	debug::DebugHandle::GetInstance()->RegisterTexture(m_Result, "edge");
#endif

	m_cbEdgeDetection = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(m_EdgeDetectionData), "EdgeDetection cb");


	m_EdgeDetectionShader = Engine::GetInstance()->GetEffect("Shaders/edge_detection.json");
	m_Quad = new Quad(m_EdgeDetectionShader);

	m_EdgeDetectionData.m_Height = m_WindowSize.m_Height;
	m_EdgeDetectionData.m_Width = m_WindowSize.m_Width;
}

void EdgeDetectionPass::Process(Texture* pTexture, const graphics::RenderContext& rc)
{
	auto& ctx = rc.GetContext();
	ctx.ClearRenderTarget(m_Result->GetRenderTargetView(), clearcolor::black);
	ctx.OMSetRenderTargets(1, m_Result->GetRenderTargetRef(), nullptr);
	ctx.UpdateConstantBuffer(m_cbEdgeDetection, &m_EdgeDetectionData);
	ctx.PSSetConstantBuffer(0, 1, &m_cbEdgeDetection);
	ctx.PSSetSamplerState(0, 1, graphics::MSAA_x16);
	m_Quad->GetShader()->AddShaderResource(pTexture, Effect::REGISTER_0);
	m_Quad->Render();


	rc.GetAPI().SetDefaultTargets();
	Effect* pEffect = Engine::GetInstance()->GetEffect("Shaders/render_to_texture.json");
	pEffect->AddShaderResource(m_Result, Effect::REGISTER_0);
	m_Quad->Render(false, pEffect);
	ctx.VSSetShaderResource(0, 1, nullptr);
	ctx.PSSetShaderResource(0, 1, nullptr);
}

void EdgeDetectionPass::OnResize()
{
	//m_ScreenQuad->OnResize();
	m_WindowSize = Engine::GetInstance()->GetInnerSize();
	m_Quad->Resize(0, 0);
}
