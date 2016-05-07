#include "stdafx.h"
#include "Texture.h"
#include <D3DX11.h>
#include "BadValueException.h"

namespace Snowblind
{
	CTexture::CTexture()
	{
	}

	CTexture::CTexture(float aWidth, float aHeight, unsigned int aBindFlag, unsigned int aFormat)
	{
		
		D3D11_TEXTURE2D_DESC tempBufferInfo;
		tempBufferInfo.Width = aWidth;
		tempBufferInfo.Height = aHeight;
		tempBufferInfo.MipLevels = 1;
		tempBufferInfo.Format = static_cast<DXGI_FORMAT>(aFormat);
		tempBufferInfo.SampleDesc.Count = 1;
		tempBufferInfo.SampleDesc.Quality = 0;
		tempBufferInfo.Usage = D3D11_USAGE_DEFAULT;
		tempBufferInfo.BindFlags = aBindFlag;
		tempBufferInfo.CPUAccessFlags = 0;
		tempBufferInfo.MiscFlags = 0;
		tempBufferInfo.ArraySize = 1;

		ID3D11Device* device = CEngine::GetDirectX()->GetDevice();

		ID3D11Texture2D* tex;

		HRESULT hr = device->CreateTexture2D(&tempBufferInfo, NULL, &tex);
		BAD_VALUE(hr != S_OK, hr);
		//CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Texture!");
		
		hr = device->CreateRenderTargetView(tex, NULL, &myRenderTargetView);
		BAD_VALUE(hr != S_OK, hr);
		//CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Texture!");

		hr = device->CreateShaderResourceView(tex, NULL, &myShaderResource);
		BAD_VALUE(hr != S_OK, hr);
		//CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Texture!");

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

	ID3D11RenderTargetView* CTexture::GetRenderTargetView()
	{
		return myRenderTargetView;
	}

	ID3D11DepthStencilView* CTexture::GetDepthView()
	{
		return myDepthStencil;
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

	void CTexture::CreateDepthStencilView(float aWidth, float aHeight, int aArraySize)
	{
		D3D11_TEXTURE2D_DESC tempBufferInfo;
		tempBufferInfo.Width = static_cast<unsigned int>(aWidth);
		tempBufferInfo.Height = static_cast<unsigned int>(aHeight);
		tempBufferInfo.MipLevels = 1;
		tempBufferInfo.ArraySize = aArraySize;
		tempBufferInfo.Format = DXGI_FORMAT_D32_FLOAT;
		tempBufferInfo.SampleDesc.Count = 1;
		tempBufferInfo.SampleDesc.Quality = 0;
		tempBufferInfo.Usage = D3D11_USAGE_DEFAULT;
		tempBufferInfo.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		tempBufferInfo.CPUAccessFlags = 0;
		tempBufferInfo.MiscFlags = 0;

		ID3D11Texture2D* tempBuffer;
		CEngine::GetDirectX()->GetDevice()->CreateTexture2D(&tempBufferInfo, NULL, &tempBuffer);
		CEngine::GetDirectX()->GetDevice()->CreateDepthStencilView(tempBuffer, NULL, &myDepthStencil);
	}

};