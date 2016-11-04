#include "stdafx.h"
#include "Texture.h"

#include <DDSTextureLoader.h>
#include <ScreenGrab.h>
namespace Snowblind
{
	CTexture::CTexture()
	{
	}

	CTexture::CTexture(float aWidth, float aHeight, unsigned int aBindFlag, unsigned int aFormat)
	{

#ifdef SNOWBLIND_DX11
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

		ID3D11Device* device = CEngine::GetAPI()->GetDevice();

		ID3D11Texture2D* tex;

		HRESULT hr = device->CreateTexture2D(&tempBufferInfo, NULL, &tex);
		CEngine::GetAPI()->HandleErrors(hr, "Failed to Create Texture!");
		hr = device->CreateRenderTargetView(tex, NULL, &myRenderTargetView);
		CEngine::GetAPI()->HandleErrors(hr, "Failed to Create Texture!");

		hr = device->CreateShaderResourceView(tex, NULL, &myShaderResource);
		CEngine::GetAPI()->HandleErrors(hr, "Failed to Create Texture!");
		tex->Release();
		tex = nullptr;
#endif
	}

	CTexture::~CTexture()
	{
#ifdef SNOWBLIND_DX11
		SAFE_RELEASE(myTexture);
		SAFE_RELEASE(myShaderResource);
		SAFE_RELEASE(myDepthTexture);
		SAFE_RELEASE(myDepthStencil);
		SAFE_RELEASE(myDepthStencilShaderView);
		SAFE_RELEASE(myRenderTargetView);
#endif
	}

	void CTexture::InitAsDepthBuffer(float aWidth, float aHeight)
	{
#ifdef SNOWBLIND_DX11
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
		HRESULT hr = CEngine::GetAPI()->GetDevice()->CreateTexture2D(&textureBufferInfo, NULL, &myTexture);
		CEngine::GetAPI()->HandleErrors(hr, "Failed to CreateTexture!");

		hr = CEngine::GetAPI()->GetDevice()->CreateRenderTargetView(myTexture, NULL, &myRenderTargetView);
		CEngine::GetAPI()->HandleErrors(hr, "Failed to Create Render Target View!");
		CreateDepthStencilView(static_cast<float>(width), static_cast<float>(height));
#endif
	}

	void CTexture::InitStencil(float aWidth, float aHeight)
	{
#ifdef SNOWBLIND_DX11
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

		HRESULT hr = CEngine::GetAPI()->GetDevice()->CreateTexture2D(&textureBufferInfo, NULL, &myDepthTexture);
		CEngine::GetAPI()->HandleErrors(hr, "Failed to CreateTexture!");



		D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
		ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthDesc.Texture2D.MipSlice = 0;

		hr = CEngine::GetAPI()->GetDevice()->CreateDepthStencilView(myDepthTexture, &depthDesc, &myDepthStencil);
		CEngine::GetAPI()->HandleErrors(hr, "Failed to create depthStencil!");

		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		viewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipLevels = 1;
		viewDesc.Texture2D.MostDetailedMip = 0;

		hr = CEngine::GetAPI()->GetDevice()->CreateShaderResourceView(myDepthTexture, &viewDesc, &myDepthStencilShaderView);
		CEngine::GetAPI()->HandleErrors(hr, "Failed to create depthstencilshaderview");
#endif
	}

	const std::string& CTexture::GetFileName()
	{
		return myFileName;
	}

	bool CTexture::LoadTexture(const std::string& aFileName, bool mip)
	{
#ifdef SNOWBLIND_DX11
		HRESULT hr;
		ID3D11Device* device = CEngine::GetAPI()->GetDevice();

		std::wstring fileName(aFileName.begin(), aFileName.end());
		
		if (!mip)
		{
			hr = DirectX::CreateDDSTextureFromFile(device, nullptr, fileName.c_str(), nullptr, &myShaderResource);
		}
		else
		{
			hr = DirectX::CreateDDSTextureFromFileEx(
				device, fileName.c_str()
				, 0
				, D3D11_USAGE_DEFAULT
				, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET
				, 0
				, D3D11_RESOURCE_MISC_GENERATE_MIPS
				, false
				, nullptr
				, &myShaderResource);

			CEngine::GetAPI()->GetContext()->GenerateMips(myShaderResource);
		};


		CEngine::GetAPI()->SetDebugName(myShaderResource, "Texture : ShaderResource_");

		if (FAILED(hr))
		{
			std::stringstream ss;
			ss << "[TEXTURE] : Failed to load texture! " << aFileName;
			OutputDebugStringA(ss.str().c_str());
			//return LoadTexture("Data/Textures/No-Texture.dds");
			return false;
		}
#endif
		return true;
	}

	void CTexture::SetDebugName(const std::string& debugName)
	{
		std::string debug(debugName);
		std::string sr = debug + "ShaderResource";
#ifdef SNOWBLIND_DX11
		CEngine::GetAPI()->SetDebugName(myShaderResource, sr);
#endif
		std::string rt = debug + "RenderTarget";
#ifdef SNOWBLIND_DX11
		CEngine::GetAPI()->SetDebugName(myRenderTargetView, rt);
#endif
		std::string ds = debug + "DepthStenci";
#ifdef SNOWBLIND_DX11
		CEngine::GetAPI()->SetDebugName(myDepthStencil, ds);
#endif
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
#ifdef SNOWBLIND_DX11
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

	HRESULT CTexture::SaveToFile(ID3D11Texture2D*& texture_resource, const std::string& file_name)
	{
		ID3D11Resource* resource = nullptr;
		HRESULT hr = texture_resource->QueryInterface(IID_ID3D11Texture2D, (void**)&resource);
		CEngine::GetAPI()->HandleErrors(hr, "Failed to query interface of texture_resource");
		std::wstring middle_hand(file_name.begin(), file_name.end());
		LPCWSTR new_name(middle_hand.c_str());
		hr = DirectX::SaveDDSTextureToFile(CEngine::GetAPI()->GetContext(), resource, new_name);
		resource->Release();
		resource = nullptr;
		return S_OK;
	}
#endif

	void CTexture::CreateDepthStencilView(float aWidth, float aHeight, int aArraySize)
	{
#ifdef SNOWBLIND_DX11
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

		DirectX11* dx = CEngine::GetAPI();
		ID3D11Device* device = dx->GetDevice();

		HRESULT hr = device->CreateTexture2D(&tempBufferInfo, NULL, &myDepthTexture);
		dx->HandleErrors(hr, "Failed to create depthTexture!");

		D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
		ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthDesc.Texture2D.MipSlice = 0;

		hr = device->CreateDepthStencilView(myDepthTexture, &depthDesc, &myDepthStencil);
		dx->HandleErrors(hr, "Failed to create depthStencil!");

		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		viewDesc.Format = DXGI_FORMAT_R32_FLOAT;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipLevels = 1;
		viewDesc.Texture2D.MostDetailedMip = 0;

		hr = CEngine::GetAPI()->GetDevice()->CreateShaderResourceView(myDepthTexture, &viewDesc, &myDepthStencilShaderView);
		dx->HandleErrors(hr, "Failed to create depthstencilshaderview");
#endif
	}
#ifdef SNOWBLIND_DX11
	void CTexture::CopyData(ID3D11Texture2D* source)
	{

		DL_ASSERT_EXP(this, "Texture was null!");
		CEngine::GetAPI()->GetContext()->CopyResource(myDepthTexture, source);
	}
#endif

};