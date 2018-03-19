#include "stdafx.h"
#include "SSAOPass.h"

#include <Engine/Engine.h>
#include <Engine/IGraphicsContext.h>
#include <Engine/RenderContext.h>
#include <Engine/Quad.h>

#ifdef _PROFILE
#include <Engine/profile_defines.h>
#endif

SSAOPass::SSAOPass()
	: IPostprocessPass()
{
	m_WindowSize = Engine::GetInstance()->GetInnerSize();

	TextureDesc desc;
	desc.m_ResourceTypeBinding = graphics::BIND_RENDER_TARGET | graphics::BIND_SHADER_RESOURCE;
	desc.m_Width = m_WindowSize.m_Width;
	desc.m_Height = m_WindowSize.m_Height;

	desc.m_TextureFormat = graphics::RGBA8_UNORM;
	desc.m_ShaderResourceFormat = graphics::RGBA8_UNORM;
	desc.m_RenderTargetFormat = graphics::RGBA8_UNORM;

	m_SSAOTexture = new Texture;
	m_SSAOTexture->Initiate(desc, false, "SSAOTexture");

	m_Effect = Engine::GetInstance()->GetEffect("Shaders/ssao.json");
	m_Quad = new Quad(m_Effect);

	m_cbSSAO = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(cbSSAO), "SSAO ConstantBuffer");

	Effect* effect = Engine::GetInstance()->GetEffect("Shaders/deferred_ambient.json");
	effect->AddShaderResource(m_SSAOTexture, Effect::SSAO);

}

SSAOPass::~SSAOPass()
{
	Engine::GetAPI()->ReleasePtr(m_cbSSAO);
	SAFE_DELETE(m_SSAOTexture);
	delete m_Quad;
	m_Quad = nullptr;
	
}

void SSAOPass::Process(Texture* scene_texture, const graphics::RenderContext& render_context)
{
	PROFILE_FUNCTION(profiler::colors::Blue);
	auto& ctx = render_context.GetContext();

	const CU::Matrix44f& projection = CU::Math::InverseReal(render_context.GetEngine().GetCamera()->GetPerspective());
	const CU::Matrix44f& view = render_context.GetEngine().GetCamera()->GetOrientation();

	m_SSAOStruct.m_Projection = projection;
	m_SSAOStruct.m_View = view;
	
	ctx.ClearRenderTarget(m_SSAOTexture->GetRenderTargetView(), clearcolor::black);
	ctx.OMSetRenderTargets(1, m_SSAOTexture->GetRenderTargetView(), nullptr);
	
	ctx.UpdateConstantBuffer(m_cbSSAO, &m_SSAOStruct, sizeof(cbSSAO));

	ctx.PSSetConstantBuffer(0, 1, &m_cbSSAO);
	ctx.PSSetSamplerState(0, 1, graphics::MSAA_x16);

	m_Effect->Use();
	ctx.DrawIndexed(m_Quad);
	m_Effect->Clear();

}

void SSAOPass::OnResize()
{

}
