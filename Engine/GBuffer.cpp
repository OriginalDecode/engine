#include "stdafx.h"
#include "GBuffer.h"

GBuffer::~GBuffer()
{
	SAFE_DELETE(myAlbedo);
	SAFE_DELETE(myNormal);
	SAFE_DELETE(myDepth);
	SAFE_DELETE(myEmissive);

}

void GBuffer::Initiate()
{
	const WindowSize windowSize = Engine::GetInstance()->GetInnerSize();

	myAlbedo = new Texture;
	myAlbedo->Initiate(windowSize.m_Width, windowSize.m_Height
		, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R16G16B16A16_FLOAT
		, "GBuffer : Albedo");

	myEmissive = new Texture;
	myEmissive->Initiate(windowSize.m_Width, windowSize.m_Height, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R8G8B8A8_UNORM, "GBuffer : Emissive");

	myNormal = new Texture;
	myNormal->Initiate(windowSize.m_Width, windowSize.m_Height, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R16G16B16A16_FLOAT, "GBuffer : Normal");

	myDepth = new Texture;
	myDepth->Initiate(windowSize.m_Width, windowSize.m_Height, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT, "GBuffer : Depth");

}

void GBuffer::Clear(float* aClearColor)
{
	render_context.m_Context->ClearRenderTargetView(myAlbedo->GetRenderTargetView(), aClearColor);
	render_context.m_Context->ClearRenderTargetView(myNormal->GetRenderTargetView(), aClearColor);
	render_context.m_Context->ClearRenderTargetView(myDepth->GetRenderTargetView(), aClearColor);
	render_context.m_Context->ClearRenderTargetView(myEmissive->GetRenderTargetView(), aClearColor);
}

void GBuffer::SetAsRenderTarget(Texture* aDepthTexture)
{
	ID3D11RenderTargetView* target[] = 
	{
		myAlbedo->GetRenderTargetView(),
		myNormal->GetRenderTargetView(),
		myDepth->GetRenderTargetView(),
		myEmissive->GetRenderTargetView(),
	};

	render_context.m_Context->OMSetRenderTargets(ARRAYSIZE(target), target, aDepthTexture->GetDepthView());
}

Texture* GBuffer::GetDiffuse() const
{
	return myAlbedo;
}

Texture* GBuffer::GetNormal() const
{
	return myNormal;
}

Texture* GBuffer::GetEmissive() const
{
	return myEmissive;
}

Texture* GBuffer::GetDepth() const
{
	return myDepth;
}

