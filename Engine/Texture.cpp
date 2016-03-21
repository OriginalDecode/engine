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
		hr = D3DX11CreateShaderResourceViewFromFile(CEngine::GetInstance()->GetAPI()->GetDevice(), aFileName, nullptr, nullptr, &myShaderResource, nullptr);

		if (FAILED(hr))
		{
			DL_WARNINGBOX_EXP(FAILED(hr), "Failed to load texture!");
			return false;
		}

		myFileName = aFileName;
		return true;
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