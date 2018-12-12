#include "stdafx.h"
#include "Shadow_Directional.h"
#include <Application/CameraHandle.h>

#include <Engine/Effect.h>
#include <Engine/Engine.h>
#include <Engine/Renderer.h>
#include <Engine/Viewport.h>
#include <Engine/Frustum.h>
#include <Engine/RenderCommand.h>
#include <Engine/Synchronizer.h>

void ShadowDirectional::Initiate(float buffer_size)
{
	m_Camera = new Camera;
	m_Camera->CreateOrthographicProjection(1024, 1024, 0.1f, 100.f);

	// 	m_Camera->SetPosition({ 512, 1, 512});
	// 	m_Camera->RotateAroundY(cl::DegreeToRad(45.f) * 1.f);
		//m_Camera->RotateAroundX(cl::DegreeToRad(90.f) * 1.f);
	//	m_Camera->Update();
	{
		TextureDesc desc;
		desc.m_Width = buffer_size;
		desc.m_Height = buffer_size;
		desc.m_Usage = graphics::DEFAULT_USAGE;
		desc.m_ResourceTypeBinding = graphics::BIND_SHADER_RESOURCE | graphics::BIND_RENDER_TARGET;
		desc.m_ShaderResourceFormat = graphics::RGBA16_FLOAT;
		desc.m_RenderTargetFormat = graphics::RGBA16_FLOAT;
		desc.m_TextureFormat = graphics::RGBA16_FLOAT;
		m_ShadowDepth = new Texture;
		m_ShadowDepth->Initiate(desc, "DirectionalShadows : RTV");
	}

	{
		m_ShadowDepthStencil = new Texture;
		m_ShadowDepthStencil->InitiateAsDepthStencil(buffer_size,buffer_size, "DirectionalShadows : DSV");

	}
	//m_ShadowDepthStencil->InitiateAsDepthStencil(buffer_size, buffer_size, "DirectionalShadows : DSV");


	m_Viewport = Engine::GetAPI()->CreateViewport(buffer_size, buffer_size, 0.f, 1.f, 0, 0);

	m_ConstBuffer.RegisterVariable(&m_Camera->GetViewProjection());
	m_ConstBuffer.Initiate("directional_shadow");

	Engine::GetInstance()->GetEffect("Shaders/deferred_ambient.json")->AddShaderResource(m_ShadowDepthStencil->GetDepthStencilView(), TextureSlot::SHADOWMAP);


#if !defined(_PROFILE) && !defined(_FINAL)
	debug::DebugHandle::GetInstance()->RegisterTexture(m_ShadowDepth, "Directional Shadow");
	debug::DebugHandle::GetInstance()->RegisterTexture(m_ShadowDepthStencil, "Directional Shadow Stencil");
#endif
	EventManager::GetInstance()->Subscribe("shadowdir.apply", this);

}

void ShadowDirectional::CleanUp()
{
	SAFE_DELETE(m_Viewport);
	SAFE_DELETE(m_Camera);
	SAFE_DELETE(m_ShadowDepth);
	SAFE_DELETE(m_ShadowDepthStencil);
}

void ShadowDirectional::SetViewport()
{
	Engine::GetAPI()->GetContext().SetViewport(m_Viewport);
}

void ShadowDirectional::ClearTexture()
{
	graphics::IGraphicsContext& ctx = Engine::GetAPI()->GetContext();
	ctx.ClearRenderTarget(m_ShadowDepth->GetRenderTargetView(), clearcolor::black);
	ctx.ClearDepthStencilView(m_ShadowDepthStencil->GetDepthView(), graphics::DEPTH | graphics::STENCIL, 1);
}

void ShadowDirectional::SetTargets()
{
	Engine::GetAPI()->GetContext().OMSetRenderTargets(1, m_ShadowDepth->GetRenderTargetRef(), m_ShadowDepthStencil->GetDepthView());
}

void ShadowDirectional::SetOrientation(const CU::Matrix44f& orientation)
{
	m_Camera->SetOrientation(orientation); 
}


void ShadowDirectional::Update()
{
	Engine* engine = Engine::GetInstance();
	Frustum& f = CameraHandle::GetInstance()->GetFrustum();
	const CU::Vector4f pos = f.GetPosition(); 
	CU::Vector3f dir = engine->GetRenderer()->GetLightDirection();
	const CU::Vector3f sun = (cl::AsVector3(pos) + (dir * ((f.GetFarPlane() - f.GetNearPlane()) / 1.7f)));

	m_Camera->LookAt(sun, cl::AsVector3(pos), CU::Vector3f(0, 1, 0)); //viewRotation

	m_Camera->SetPosition(sun);
	m_Camera->UpdateOrthographicProjection(f);

	m_Camera->Update();

	const CU::Vector4f cam_pos = m_Camera->GetTranslation();
	const CU::Vector4f cam_dir = m_Camera->GetAt();

	Engine::GetInstance()->GetSynchronizer()->AddRenderCommand(LineCommand(cam_pos + cam_dir, f.GetCenter(), CU::Vector4f(1, 0, 0, 1), true));
	Engine::GetInstance()->GetSynchronizer()->AddRenderCommand(LineCommand(cam_pos, cam_pos + cam_dir, CU::Vector4f(0, 1, 0, 1), true));

}

CU::Matrix44f ShadowDirectional::GetMVP()
{
	const CU::Matrix44f& orientation = CU::Math::Inverse(m_Camera->GetRotation() * m_Camera->GetOrientation());
	const CU::Matrix44f& perspective = m_Camera->GetOrthographic();
	return orientation * perspective;
}

void ShadowDirectional::HandleEvent(uint64 event, void* pData)
{
}
