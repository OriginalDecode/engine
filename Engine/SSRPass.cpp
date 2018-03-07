#include "stdafx.h"
#include "SSRPass.h"

#include <Engine/Engine.h>
#include <Engine/IGraphicsContext.h>
#include <Engine/RenderContext.h>
#include <Engine/Quad.h>

#ifdef _PROFILE
#include <Engine/profile_defines.h>
#endif

SSRPass::SSRPass()
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

	m_Reflection = new Texture;
	m_Reflection->Initiate(desc, false, "SSR Texture");

	m_Effect = Engine::GetInstance()->GetEffect("Data/Shaders/ssr.json");
	m_Quad = new Quad(m_Effect);

	m_Buffer = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(m_SSRStruct), "SSR Buffer");
/*
	Effect* effect = Engine::GetInstance()->GetEffect("Shaders/deferred_ambient.json");
	effect->AddShaderResource(m_Reflection, Effect::SSR);*/

	debug::DebugHandle* pDebug = debug::DebugHandle::GetInstance();
	pDebug->RegisterTexture(m_Reflection, "SSR");


}


SSRPass::~SSRPass()
{
	delete m_Reflection;
	m_Reflection = nullptr;
}

void SSRPass::Process(Texture* scene, const graphics::RenderContext& rc)
{
	PROFILE_FUNCTION(profiler::colors::Blue);
	auto& ctx = rc.GetContext();

	const CU::Matrix44f& projection = CU::Math::InverseReal(rc.GetEngine().GetCamera()->GetPerspective());
	const CU::Matrix44f& view = rc.GetEngine().GetCamera()->GetOrientation();

	m_SSRStruct.m_InvProjection = projection;
	m_SSRStruct.m_Projection = rc.GetEngine().GetCamera()->GetPerspective();
	m_SSRStruct.m_InvView = view;
	m_SSRStruct.m_View = CU::Math::Inverse(view);
	m_SSRStruct.m_CameraPos = rc.GetEngine().GetCamera()->GetPos();

	ctx.ClearRenderTarget(m_Reflection->GetRenderTargetView(), clearcolor::black);
	ctx.OMSetRenderTargets(1, m_Reflection->GetRenderTargetRef(), nullptr);

	ctx.UpdateConstantBuffer(m_Buffer, &m_SSRStruct, sizeof(m_SSRStruct));

	ctx.PSSetConstantBuffer(0, 1, &m_Buffer);
	ctx.PSSetSamplerState(0, 1, graphics::MSAA_x16);

	rc.GetAPI().SetDefaultTargets();

	m_Effect->AddShaderResource(scene, Effect::ALBEDO);
	m_Effect->Use();
	ctx.DrawIndexed(m_Quad);
	m_Effect->Clear();
}

void SSRPass::OnResize()
{

}
