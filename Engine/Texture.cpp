#include "stdafx.h"
#include "Texture.h"
#include <D3DX11.h>

namespace Snowblind
{
	CTexture::CTexture()
	{
	}

	CTexture::~CTexture()
	{
		if (myShaderResource != nullptr)
		{
			myShaderResource->Release();
		}
		myShaderResource = nullptr;
	}

	const char* CTexture::GetFileName()
	{
		return myFileName;
	}

	bool CTexture::LoadTexture(const char* aFileName)
	{
		HRESULT hr;
		myFileName = aFileName;
		hr = D3DX11CreateShaderResourceViewFromFile(CEngine::GetInstance()->GetAPI()->GetDevice(), myFileName, nullptr, nullptr, &myShaderResource, nullptr);
		if (FAILED(hr))
		{
			DL_WARNINGBOX_EXP(FAILED(hr), "Failed to load texture!");
			return false;
		}



		return true;
	}

	ID3D11ShaderResourceView* CTexture::GetShaderView()
	{
		return myShaderResource;
	}

	void CTexture::SetTexture(ID3D11ShaderResourceView* aShaderResource)
	{
		myShaderResource = aShaderResource;
	}

	void CTexture::SaveToFile(const char* aFileName, int aSize)
	{
		D3D11_TEXTURE2D_DESC tempBufferInfo;
		tempBufferInfo.Width = aSize;
		tempBufferInfo.Height = aSize;
		tempBufferInfo.MipLevels = 1;
		tempBufferInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tempBufferInfo.SampleDesc.Count = 1;
		tempBufferInfo.SampleDesc.Quality = 0;
		tempBufferInfo.Usage = D3D11_USAGE_DEFAULT;
		tempBufferInfo.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		tempBufferInfo.CPUAccessFlags = 0;
		tempBufferInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		ID3D11Resource *resource;
		myShaderResource->GetResource(&resource);
		HRESULT hr = S_OK;
		hr = D3DX11SaveTextureToFile(CEngine::GetInstance()->GetAPI()->GetContext(), resource, D3DX11_IMAGE_FILE_FORMAT::D3DX11_IFF_DDS, aFileName);
		CEngine::GetInstance()->GetAPI()->HandleErrors(hr, "Failed to save Texture! ");
		resource->Release();
	}
};