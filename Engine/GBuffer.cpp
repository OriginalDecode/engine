#include "stdafx.h"
#include "GBuffer.h"
#include <Engine/engine_shared.h>
#include <Engine/IGraphicsContext.h>
#include <Engine/IGraphicsAPI.h>
namespace graphics
{

	GBuffer::GBuffer()
	{
		const WindowSize windowSize = Engine::GetInstance()->GetInnerSize();
		const float window_width = windowSize.m_Width;
		const float window_height = windowSize.m_Height;

		TextureDesc desc;
		desc.m_Width = window_width;
		desc.m_Height = window_height;
		desc.m_Usage = graphics::DEFAULT_USAGE;
		desc.m_ResourceTypeBinding = graphics::BIND_SHADER_RESOURCE | graphics::BIND_RENDER_TARGET;
		desc.m_ShaderResourceFormat = RGBA16_FLOAT;
		desc.m_RenderTargetFormat = RGBA16_FLOAT;
		m_Albedo = new Texture;
		m_Albedo->Initiate(desc, "GBuffer : Albedo");

		m_Emissive = new Texture;
		m_Emissive->Initiate(desc, "GBuffer : Emissive");

		m_Normal = new Texture;
		m_Normal->Initiate(desc, "GBuffer : Normal");

		m_Depth = new Texture;
		desc.m_ShaderResourceFormat = RGBA32_FLOAT;
		desc.m_RenderTargetFormat = RGBA32_FLOAT;
		m_Depth->Initiate(desc, "GBuffer : Depth");

		Effect* shader = Engine::GetInstance()->GetEffect("Shaders/deferred_ambient.json");
		shader->AddShaderResource(m_Albedo, Effect::DIFFUSE);
		shader->AddShaderResource(m_Normal, Effect::NORMAL);
		shader->AddShaderResource(m_Depth, Effect::DEPTH);
		shader->AddShaderResource(m_Emissive, Effect::EMISSIVE);
	}

	GBuffer::~GBuffer()
	{
		SAFE_DELETE(m_Albedo);
		SAFE_DELETE(m_Normal);
		SAFE_DELETE(m_Depth);
		SAFE_DELETE(m_Emissive);
	}

	void GBuffer::Clear(float clear_color[4], const RenderContext& render_context)
	{
		auto& ctx = render_context.GetContext();
		ctx.ClearRenderTarget(m_Albedo->GetRenderTargetView(), clear_color);
		ctx.ClearRenderTarget(m_Normal->GetRenderTargetView(), clear_color);
		ctx.ClearRenderTarget(m_Depth->GetRenderTargetView(), clear_color);
		ctx.ClearRenderTarget(m_Emissive->GetRenderTargetView(), clear_color);
	}

	void GBuffer::SetAsRenderTarget(Texture* depth, const RenderContext& render_context)
	{
		IRenderTargetView* target[] =
		{
			m_Albedo->GetRenderTargetView(),
			m_Normal->GetRenderTargetView(),
			m_Depth->GetRenderTargetView(),
			m_Emissive->GetRenderTargetView(),
		};

		render_context.GetContext().OMSetRenderTargets(ARRAYSIZE(target), target, depth->GetDepthView());
	}

};