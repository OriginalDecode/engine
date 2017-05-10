#include "stdafx.h"
#include "GBuffer.h"

void GBuffer::Initiate()
{
	const WindowSize windowSize = Engine::GetInstance()->GetWindowSize();

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

void GBuffer::CleanUp()
{
	myAlbedo->CleanUp();
	SAFE_DELETE(myAlbedo);

	myEmissive->CleanUp();
	SAFE_DELETE(myEmissive);

	myNormal->CleanUp();
	SAFE_DELETE(myNormal);

	myDepth->CleanUp();
	SAFE_DELETE(myDepth);
}

void GBuffer::Clear(float* aClearColor)
{
	myContext->ClearRenderTargetView(myAlbedo->GetRenderTargetView(), aClearColor);
	myContext->ClearRenderTargetView(myNormal->GetRenderTargetView(), aClearColor);
	myContext->ClearRenderTargetView(myDepth->GetRenderTargetView(), aClearColor);
	myContext->ClearRenderTargetView(myEmissive->GetRenderTargetView(), aClearColor);
}

void GBuffer::SetAsRenderTarget(Texture* aDepthTexture)
{
	ID3D11RenderTargetView* target[4];
	target[0] = myAlbedo->GetRenderTargetView();
	target[1] = myNormal->GetRenderTargetView();
	target[2] = myDepth->GetRenderTargetView();
	target[3] = myEmissive->GetRenderTargetView();
	myContext->OMSetRenderTargets(4, target, aDepthTexture->GetDepthView());
}

Texture* GBuffer::GetDiffuse()
{
	return myAlbedo;
}

Texture* GBuffer::GetNormal()
{
	return myNormal;
}

Texture* GBuffer::GetEmissive()
{
	return myEmissive;
}

Texture* GBuffer::GetDepth()
{
	return myDepth;
}

