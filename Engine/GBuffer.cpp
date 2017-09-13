#include "stdafx.h"
#include "GBuffer.h"
#include <Engine/engine_shared.h>
#include <Engine/IGraphicsContext.h>
#include <Engine/IGraphicsAPI.h>
namespace graphics
{
	GBuffer::~GBuffer()
	{
		SAFE_DELETE(m_Albedo);
		SAFE_DELETE(m_Normal);
		SAFE_DELETE(m_Depth);
		SAFE_DELETE(m_Emissive);
	}

	void GBuffer::Initiate()
	{
		const WindowSize windowSize = Engine::GetInstance()->GetInnerSize();

		m_Albedo = new Texture;
		/*myAlbedo->Initiate(windowSize.m_Width, windowSize.m_Height
			, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R16G16B16A16_FLOAT
			, "GBuffer : Albedo");
	*/
		m_Emissive = new Texture;
		//myEmissive->Initiate(windowSize.m_Width, windowSize.m_Height, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R8G8B8A8_UNORM, "GBuffer : Emissive");

		m_Normal = new Texture;
		//myNormal->Initiate(windowSize.m_Width, windowSize.m_Height, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R16G16B16A16_FLOAT, "GBuffer : Normal");

		m_Depth = new Texture;
		//myDepth->Initiate(windowSize.m_Width, windowSize.m_Height, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT, "GBuffer : Depth");

		Effect* shader = Engine::GetInstance()->GetEffect("Shaders/deferred_ambient.json");
		shader->AddShaderResource(m_Albedo, Effect::DIFFUSE);
		shader->AddShaderResource(m_Normal, Effect::NORMAL);
		shader->AddShaderResource(m_Depth, Effect::DEPTH);
		shader->AddShaderResource(m_Emissive, Effect::EMISSIVE);


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