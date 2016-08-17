#include "stdafx.h"
#include "Texture.h"

#include <DDSTextureLoader.h>

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
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Texture!");
		hr = device->CreateRenderTargetView(tex, NULL, &myRenderTargetView);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Texture!");
		CEngine::GetDirectX()->SetDebugName(myRenderTargetView, "Texture RTV");

		hr = device->CreateShaderResourceView(tex, NULL, &myShaderResource);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Texture!");
		CEngine::GetDirectX()->SetDebugName(myShaderResource, "Texture SRV");
		tex->Release();
		tex = nullptr;
	}

	CTexture::~CTexture()
	{
		SAFE_RELEASE(myDepthStencil);
		SAFE_RELEASE(myShaderResource);
		SAFE_RELEASE(myRenderTargetView);
		SAFE_RELEASE(myDepthStencilShaderView);
		SAFE_RELEASE(myDepthTexture);
		SAFE_RELEASE(myTexture);
	}

	void CTexture::InitAsDepthBuffer(float aWidth, float aHeight)
	{
		int width = static_cast<int>(aWidth);
		int height = static_cast<int>(aHeight);

		myFileName = "Initied as DSV";
		myRenderTargetView = nullptr;
		myShaderResource = nullptr;
		myDepthStencil = nullptr;
		myDepthStencilShaderView = nullptr;
		myDepthTexture = nullptr;

		D3D11_TEXTURE2D_DESC textureBufferInfo;
		textureBufferInfo.Width = width;
		textureBufferInfo.Height = height;
		textureBufferInfo.MipLevels = 1;
		textureBufferInfo.ArraySize = 1;
		textureBufferInfo.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		textureBufferInfo.SampleDesc.Count = 1;
		textureBufferInfo.SampleDesc.Quality = 0;
		textureBufferInfo.Usage = D3D11_USAGE_DEFAULT;
		int flags = 0;
		flags |= D3D11_BIND_SHADER_RESOURCE;
		flags |= D3D11_BIND_RENDER_TARGET;

		textureBufferInfo.BindFlags = flags;
		textureBufferInfo.CPUAccessFlags = 0;
		textureBufferInfo.MiscFlags = 0;
		HRESULT hr = CEngine::GetDirectX()->GetDevice()->CreateTexture2D(&textureBufferInfo, NULL, &myTexture);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to CreateTexture!");

		hr = CEngine::GetDirectX()->GetDevice()->CreateRenderTargetView(myTexture, NULL, &myRenderTargetView);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Create Render Target View!");
		CEngine::GetDirectX()->SetDebugName(myRenderTargetView, "Texture : RenderTargetView");
		CreateDepthStencilView(static_cast<float>(width), static_cast<float>(height));
	}

	void CTexture::InitStencil(float aWidth, float aHeight)
	{
		int width = static_cast<int>(aWidth);
		int height = static_cast<int>(aHeight);

		myFileName = "Initied as DSV";
		myRenderTargetView = nullptr;
		myShaderResource = nullptr;
		myDepthStencil = nullptr;
		myDepthStencilShaderView = nullptr;
		myDepthTexture = nullptr;

		D3D11_TEXTURE2D_DESC textureBufferInfo;
		textureBufferInfo.Width = width;
		textureBufferInfo.Height = height;
		textureBufferInfo.MipLevels = 1;
		textureBufferInfo.ArraySize = 1;

		textureBufferInfo.Format = DXGI_FORMAT_R24G8_TYPELESS;
		textureBufferInfo.SampleDesc.Count = 1;
		textureBufferInfo.SampleDesc.Quality = 0;
		textureBufferInfo.Usage = D3D11_USAGE_DEFAULT;
		textureBufferInfo.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		textureBufferInfo.CPUAccessFlags = 0;
		textureBufferInfo.MiscFlags = 0;

		HRESULT hr = CEngine::GetDirectX()->GetDevice()->CreateTexture2D(&textureBufferInfo, NULL, &myDepthTexture);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to CreateTexture!");



		D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
		ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthDesc.Texture2D.MipSlice = 0;

		hr = CEngine::GetDirectX()->GetDevice()->CreateDepthStencilView(myDepthTexture, &depthDesc, &myDepthStencil);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to create depthStencil!");
		CEngine::GetDirectX()->SetDebugName(myDepthStencil, "Texture : DepthStencil");

		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		viewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipLevels = 1;
		viewDesc.Texture2D.MostDetailedMip = 0;

		hr = CEngine::GetDirectX()->GetDevice()->CreateShaderResourceView(myDepthTexture, &viewDesc, &myDepthStencilShaderView);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to create depthstencilshaderview");
		CEngine::GetDirectX()->SetDebugName(myDepthStencilShaderView, "Texture : DepthStencilShaderView");

	}

	const std::string& CTexture::GetFileName()
	{
		return myFileName;
	}

	bool CTexture::LoadTexture(const std::string& aFileName)
	{
		HRESULT hr;
		ID3D11Device* device = CEngine::GetDirectX()->GetDevice();

		std::wstring fileName(aFileName.begin(), aFileName.end());
		hr = DirectX::CreateDDSTextureFromFile(device, nullptr, fileName.c_str(), nullptr, &myShaderResource);
		CEngine::GetDirectX()->SetDebugName(myShaderResource, "Texture : ShaderResource_");

		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "[TEXTURE] : Failed to load texture! " << aFileName;
			OutputDebugStringA(ss.str().c_str());
			return LoadTexture("Data/Textures/No-Texture.dds");
			return false;
		}
		return true;
	}

	void CTexture::SetDebugName(const std::string& debugName)
	{
		std::string debug(debugName);
		std::string sr = debug + "ShaderResource";
		CEngine::GetDirectX()->SetDebugName(myShaderResource, sr);

		std::string rt = debug + "RenderTarget";
		CEngine::GetDirectX()->SetDebugName(myRenderTargetView, rt);

		std::string ds = debug + "DepthStenci";
		CEngine::GetDirectX()->SetDebugName(myDepthStencil, ds);
		//static_assert(false,"Missing stuff");
	}

	void CTexture::CreateTexture(const std::string& aTexturePath)
	{
		std::ifstream stream(aTexturePath.c_str(), std::ios::binary | std::ios::ate | std::ios::in);
		std::ifstream::pos_type pos = stream.tellg();

		std::vector<char> result(pos);

		stream.seekg(0, std::ios::beg);
		stream.read(&result[0], pos);


		//ID3D11Resource* resource = NULL;
		//HRESULT hr = S_OK;

		//hr = D3DX11CreateTextureFromMemory(CEngine::GetDirectX()->GetDevice(), &result[0], result.size(), NULL, NULL, &resource, 0);
		//hr = D3DX11CreateShaderResourceViewFromMemory(CEngine::GetDirectX()->GetDevice(), &result[0], result.size(), 0, 0, &myShaderResource, &hr);
		//CEngine::GetDirectX()->HandleErrors(hr, "Failed to create texture from memory");
	}

	ID3D11Texture2D* CTexture::GetDepthTexture()
	{
		return myDepthTexture;
	}

	ID3D11ShaderResourceView* CTexture::GetShaderView()
	{
		return myShaderResource;
	}

	ID3D11ShaderResourceView* CTexture::GetDepthStencilView()
	{
		return myDepthStencilShaderView;
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
		/*D3D11_TEXTURE2D_DESC tempBufferInfo;
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

		ID3D11Resource* resource;
		myShaderResource->GetResource(&resource);
		HRESULT hr = S_OK;
		hr = D3DX11SaveTextureToFile(CEngine::GetDirectX()->GetContext(), resource, D3DX11_IMAGE_FILE_FORMAT::D3DX11_IFF_DDS, aFileName);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to save Texture! ");
		resource->Release();*/
	}

	void CTexture::CreateDepthStencilView(float aWidth, float aHeight, int aArraySize)
	{
		D3D11_TEXTURE2D_DESC tempBufferInfo;
		tempBufferInfo.Width = static_cast<unsigned int>(aWidth);
		tempBufferInfo.Height = static_cast<unsigned int>(aHeight);
		tempBufferInfo.MipLevels = 1;
		tempBufferInfo.ArraySize = aArraySize;
		tempBufferInfo.Format = DXGI_FORMAT_R32_TYPELESS;
		tempBufferInfo.SampleDesc.Count = 1;
		tempBufferInfo.SampleDesc.Quality = 0;
		tempBufferInfo.Usage = D3D11_USAGE_DEFAULT;
		tempBufferInfo.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		tempBufferInfo.CPUAccessFlags = 0;
		tempBufferInfo.MiscFlags = 0;

		CDirectX11* dx = CEngine::GetDirectX();
		ID3D11Device* device = dx->GetDevice();

		HRESULT hr = device->CreateTexture2D(&tempBufferInfo, NULL, &myDepthTexture);
		dx->HandleErrors(hr, "Failed to create depthTexture!");
		dx->SetDebugName(myDepthTexture, "Texture : DepthTexture");

		D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
		ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthDesc.Texture2D.MipSlice = 0;

		hr = device->CreateDepthStencilView(myDepthTexture, &depthDesc, &myDepthStencil);
		dx->HandleErrors(hr, "Failed to create depthStencil!");
		dx->SetDebugName(myDepthStencil, "Texture : DepthStencil");

		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		viewDesc.Format = DXGI_FORMAT_R32_FLOAT;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipLevels = 1;
		viewDesc.Texture2D.MostDetailedMip = 0;

		hr = CEngine::GetDirectX()->GetDevice()->CreateShaderResourceView(myDepthTexture, &viewDesc, &myDepthStencilShaderView);
		dx->HandleErrors(hr, "Failed to create depthstencilshaderview");
		dx->SetDebugName(myDepthStencilShaderView, "Texture : DepthStencilShaderView");


	}

	void CTexture::CopyData(ID3D11Texture2D* source)
	{
		DL_ASSERT_EXP(this, "Texture was null!");
		CEngine::GetDirectX()->GetContext()->CopyResource(myDepthTexture, source);
	}

};