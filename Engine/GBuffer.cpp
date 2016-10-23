#include "stdafx.h"
#include "GBuffer.h"
namespace Snowblind
{
	CGBuffer::CGBuffer()
		: myEngine(CEngine::GetInstance())
#ifdef SNOWBLIND_DX11
		, myDirectX(myEngine->GetAPI())
		, myContext(myDirectX->GetContext())
#endif
	{
		SWindowSize windowSize = CEngine::GetInstance()->GetWindowSize();
		myAlbedo = new CTexture(windowSize.myWidth, windowSize.myHeight
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R16G16B16A16_FLOAT);
		myAlbedo->SetDebugName("Deferred_Albedo");

		myEmissive = new CTexture(windowSize.myWidth, windowSize.myHeight
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM);
		myEmissive->SetDebugName("Deferred_Emissive");

		myNormal = new CTexture(windowSize.myWidth, windowSize.myHeight
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM);
		myNormal->SetDebugName("Deferred_Normal");

		myDepth = new CTexture(windowSize.myWidth, windowSize.myHeight
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R32G32B32A32_FLOAT);
		myDepth->SetDebugName("Deferred_Depth");
	}
	
	CGBuffer::~CGBuffer()
	{
		SAFE_DELETE(myAlbedo);
		SAFE_DELETE(myEmissive);
		SAFE_DELETE(myNormal);
		SAFE_DELETE(myDepth);
	}
	
	void CGBuffer::Clear(float* aClearColor)
	{
#ifdef SNOWBLIND_DX11
		myContext->ClearRenderTargetView(myAlbedo->GetRenderTargetView(), aClearColor);
		myContext->ClearRenderTargetView(myNormal->GetRenderTargetView(), aClearColor);
		myContext->ClearRenderTargetView(myDepth->GetRenderTargetView(), aClearColor);
		myContext->ClearRenderTargetView(myEmissive->GetRenderTargetView(), aClearColor);
#endif
	}

	void CGBuffer::SetAsRenderTarget(CTexture* aDepthTexture)
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