#include "stdafx.h"
#include "GBuffer.h"
namespace Snowblind
{
	GBuffer::GBuffer()
		: myEngine(Engine::GetInstance())
#ifdef SNOWBLIND_DX11
		, myDirectX(myEngine->GetAPI())
		, myContext(myDirectX->GetContext())
#endif
	{
		const SWindowSize windowSize = Engine::GetInstance()->GetWindowSize();

		myAlbedo = new Texture;
		myAlbedo->Initiate(windowSize.myWidth, windowSize.myHeight, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R16G16B16A16_FLOAT, "GBuffer : Albedo");

		myEmissive = new Texture;
		myEmissive->Initiate(windowSize.myWidth, windowSize.myHeight, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R8G8B8A8_UNORM, "GBuffer : Emissive");

		myNormal = new Texture;
		myNormal->Initiate(windowSize.myWidth, windowSize.myHeight, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R16G16B16A16_FLOAT, "GBuffer : Normal");
		
		myDepth = new Texture;
		myDepth->Initiate(windowSize.myWidth, windowSize.myHeight, DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT, "GBuffer : Depth");


	}
	
	GBuffer::~GBuffer()
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
#ifdef SNOWBLIND_DX11
		myContext->ClearRenderTargetView(myAlbedo->GetRenderTargetView(), aClearColor);
		myContext->ClearRenderTargetView(myNormal->GetRenderTargetView(), aClearColor);
		myContext->ClearRenderTargetView(myDepth->GetRenderTargetView(), aClearColor);
		myContext->ClearRenderTargetView(myEmissive->GetRenderTargetView(), aClearColor);
#endif
	}

	void GBuffer::SetAsRenderTarget(Texture* aDepthTexture)
	{
#ifdef SNOWBLIND_DX11
		ID3D11RenderTargetView* target[4];
		target[0] = myAlbedo->GetRenderTargetView();
		target[1] = myNormal->GetRenderTargetView();
		target[2] = myDepth->GetRenderTargetView();
		target[3] = myEmissive->GetRenderTargetView();
		myContext->OMSetRenderTargets(4, target, aDepthTexture->GetDepthView());
#endif
	}

};