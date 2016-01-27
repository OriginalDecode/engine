#include "Texture.h"
#include "DirectX11.h"
#include "API.h"
#include <D3DX11.h>
#include "Engine.h"
#include <DL_Debug.h>

namespace Snowblind
{
	CTexture::CTexture()
	{
	}


	CTexture::~CTexture()
	{
	}

	const char* CTexture::GetFileName()
	{
		return myFileName;
	}

	bool CTexture::LoadTexture(const char* aFileName)
	{
		HRESULT hr;
		hr = D3DX11CreateShaderResourceViewFromFile(reinterpret_cast<CDirectX11*>(CEngine::GetInstance()->GetAPI())->GetDevice(), aFileName, nullptr, nullptr, &myShaderResource, nullptr);
		DL_WARNINGBOX_EXP(FAILED(hr), "Failed to load texture!");
		if (!FAILED(hr))
		{
			myFileName = aFileName;
			return true;
		}
		return false;
	}

	ID3D11ShaderResourceView* CTexture::GetShaderView()
	{
		return myShaderResource;
	}

	ID3D11RenderTargetView* CTexture::GetRenderTargetView()
	{
		return myRenderTarget;
	}

	ID3D11RenderTargetView** CTexture::GetRenderTarget()
	{
		return &myRenderTarget;
	}

};