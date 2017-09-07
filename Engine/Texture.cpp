#include "stdafx.h"
#include "Texture.h"
#include <Engine/IGraphicsDevice.h>

Texture::~Texture()
{
	graphics::IGraphicsDevice& device = Engine::GetAPI()->GetDevice();
	device.ReleasePtr(m_ShaderResource);
	device.ReleasePtr(m_DepthTexture);
	device.ReleasePtr(m_DepthStencilView);
	device.ReleasePtr(m_DepthStencilShaderView);
	device.ReleasePtr(m_RenderTargetView);
}

void Texture::Initiate(u16 width, u16 height, s32 usage, s32 binding, graphics::eTextureFormat texture_format, const std::string& debug_name)
{

	m_Width = width;
	m_Height = height;

	graphics::Texture2DDesc desc;
	desc.m_Height = m_Height;
	desc.m_Width = m_Width;
	desc.m_Format = texture_format;
	desc.m_MipLevels = 0;
	desc.m_ArraySize = 0;
	desc.m_Usage = usage;
	desc.m_MiscFlags = 0;
	desc.m_SampleCount = 1;
	desc.m_SampleQuality = 0;
	desc.m_AccessCPU = false;
	desc.m_Binding = binding;

	graphics::IGraphicsDevice& device = Engine::GetAPI()->GetDevice();
	void* output = device.CreateTexture2D(m_DepthTexture, desc, debug_name.c_str());





// 	graphics::DirectX11* api = static_cast<graphics::DirectX11*>(Engine::GetAPI());
// 	ID3D11Device* device = api->GetDevice();
// 
// 	D3D11_TEXTURE2D_DESC text_desc;
// 	text_desc.Width = (UINT)width;
// 	text_desc.Height = (UINT)height;
// 	text_desc.MipLevels = 1;
// 	text_desc.Format = texture_format;
// 	text_desc.SampleDesc.Count = 1;
// 	text_desc.SampleDesc.Quality = 0;
// 	text_desc.Usage = GetUsage(flags);
// 	text_desc.BindFlags = (flags & ~(DEFAULT_USAGE | IMMUTABLE_USAGE | DYNAMIC_USAGE | STAGING_USAGE));
// 	text_desc.CPUAccessFlags = 0;
// 	text_desc.MiscFlags = 0;
// 	text_desc.ArraySize = 1;
// 	HRESULT hr = device->CreateTexture2D(&text_desc, NULL, &m_DepthTexture);
// 	api->HandleErrors(hr, "[Texture](Initiate) : Failed to initiate texture.");
// 
// 	if (flags & D3D11_BIND_RENDER_TARGET)
// 	{
// 		hr = device->CreateRenderTargetView(m_DepthTexture, NULL, &m_RenderTargetView);
// 		api->HandleErrors(hr, "[Texture](Initiate) : Failed to create RenderTargetView.");
// 		api->SetDebugName(m_RenderTargetView, debug_name + "RenderTargetView");
// 	}
// 
// 	if (flags & D3D11_BIND_SHADER_RESOURCE)
// 	{
// 		hr = device->CreateShaderResourceView(m_DepthTexture, NULL, &m_ShaderResource);
// 		api->HandleErrors(hr, "[Texture](Initiate) : Failed to create ShaderResourceView.");
// 		api->SetDebugName(m_ShaderResource, debug_name + "ShaderResourceView");
// 	}
// 
// 	if (flags & D3D11_BIND_DEPTH_STENCIL)
// 	{
// 		DL_ASSERT("Invalid flag D3D11_BIND_DEPTH_STENCIL for cr'eating regular texture!");
// 	}

}

void Texture::Initiate(u16 width, u16 height, s32 flags, graphics::eTextureFormat texture_format, graphics::eTextureFormat shader_resource_view_format, graphics::eTextureFormat depth_stencil_format, const std::string& debug_name)
{

	m_Width = width;
	m_Height = height;
	DirectX11* api = Engine::GetAPI();
	ID3D11Device* device = Engine::GetAPI()->GetDevice();

	D3D11_TEXTURE2D_DESC text_desc;
	text_desc.Width = (UINT)width;
	text_desc.Height = (UINT)height;
	text_desc.MipLevels = 1;
	text_desc.Format = texture_format;
	text_desc.SampleDesc.Count = 1;
	text_desc.SampleDesc.Quality = 0;
	text_desc.Usage = GetUsage(flags);
	text_desc.BindFlags = (flags & ~(DEFAULT_USAGE | IMMUTABLE_USAGE | DYNAMIC_USAGE | STAGING_USAGE));
	text_desc.CPUAccessFlags = 0;
	text_desc.MiscFlags = 0;
	text_desc.ArraySize = 1;

	HRESULT hr = device->CreateTexture2D(&text_desc, NULL, &m_DepthTexture);
	api->HandleErrors(hr, "[Texture](Initiate) : Failed to initiate texture.");
	api->SetDebugName(m_DepthTexture, debug_name + "Texture");

	if (flags & D3D11_BIND_RENDER_TARGET)
	{
		DL_ASSERT("Invalid flag D3D11_BIND_RENDER_TARGET for creating DepthTexture.");
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
	ZeroMemory(&view_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	view_desc.Format = shader_resource_view_format;
	view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	view_desc.Texture2D.MipLevels = 1;
	view_desc.Texture2D.MostDetailedMip = 0;

	if (flags & D3D11_BIND_SHADER_RESOURCE && !m_DepthStencilShaderView)
	{
		hr = device->CreateShaderResourceView(m_DepthTexture, &view_desc, &m_DepthStencilShaderView);
		api->HandleErrors(hr, "[Texture](Initiate) : Failed to create DepthStencil-ShaderResourceView.");
		api->SetDebugName(m_DepthStencilShaderView, debug_name + "DepthStencil-ShaderResourceView");
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depth_desc;
	ZeroMemory(&depth_desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depth_desc.Format = depth_stencil_format;
	depth_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_desc.Texture2D.MipSlice = 0;

	if (flags & D3D11_BIND_DEPTH_STENCIL && !m_DepthStencilView)
	{
		hr = device->CreateDepthStencilView(m_DepthTexture, &depth_desc, &m_DepthStencilView);
		api->HandleErrors(hr, "[Texture](Initiate) : Failed to create Depthstencil-DepthStencilView.");
		api->SetDebugName(m_DepthStencilView, debug_name + "DepthStencilView");
	}

}

void Texture::Initiate(u16 width, u16 height, s32 flags, graphics::eTextureFormat render_target_format, graphics::eTextureFormat texture_format, graphics::eTextureFormat shader_resource_view_format, graphics::eTextureFormat depth_stencil_format, const std::string& debug_name)
{

	myWidth = width;
	myHeight = height;
	DirectX11* api = Engine::GetAPI();
	ID3D11Device* device = Engine::GetAPI()->GetDevice();

	D3D11_TEXTURE2D_DESC text_desc;
	text_desc.Width = (UINT)width;
	text_desc.Height = (UINT)height;
	text_desc.MipLevels = 1;
	text_desc.Format = texture_format;
	text_desc.SampleDesc.Count = 1;
	text_desc.SampleDesc.Quality = 0;
	text_desc.Usage = GetUsage(flags);
	text_desc.BindFlags = (flags & ~(DEFAULT_USAGE | IMMUTABLE_USAGE | DYNAMIC_USAGE | STAGING_USAGE | D3D11_BIND_RENDER_TARGET));;
	text_desc.CPUAccessFlags = 0;
	text_desc.MiscFlags = 0;
	text_desc.ArraySize = 1;

	HRESULT hr = device->CreateTexture2D(&text_desc, NULL, &m_DepthTexture);
	api->HandleErrors(hr, "[Texture](Initiate) : Failed to initiate texture.");
	api->SetDebugName(m_DepthTexture, debug_name + "Texture");

	if (flags & D3D11_BIND_RENDER_TARGET)
	{
		D3D11_TEXTURE2D_DESC rt_text_desc;
		rt_text_desc.Width = (UINT)width;
		rt_text_desc.Height = (UINT)height;
		rt_text_desc.MipLevels = 1;
		rt_text_desc.Format = render_target_format;
		rt_text_desc.SampleDesc.Count = 1;
		rt_text_desc.SampleDesc.Quality = 0;
		rt_text_desc.Usage = GetUsage(flags);
		rt_text_desc.BindFlags = (flags & ~(DEFAULT_USAGE | IMMUTABLE_USAGE | DYNAMIC_USAGE | STAGING_USAGE | D3D11_BIND_DEPTH_STENCIL));
		rt_text_desc.CPUAccessFlags = 0;
		rt_text_desc.MiscFlags = 0;
		rt_text_desc.ArraySize = 1;

		ID3D11Texture2D* texture = nullptr;
		hr = device->CreateTexture2D(&rt_text_desc, NULL, &texture);
		api->HandleErrors(hr, "[Texture](Initiate) : Failed to initiate texture.");

		hr = device->CreateRenderTargetView(texture, NULL, &m_RenderTargetView);
		api->HandleErrors(hr, "[Texture](Initiate) : Failed to create RenderTargetView.");
		api->SetDebugName(m_RenderTargetView, debug_name + "RenderTargetView");
		SAFE_RELEASE(texture);
	}

	if (flags & D3D11_BIND_SHADER_RESOURCE)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
		ZeroMemory(&view_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		view_desc.Format = shader_resource_view_format;
		view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		view_desc.Texture2D.MipLevels = 1;
		view_desc.Texture2D.MostDetailedMip = 0;

		hr = device->CreateShaderResourceView(m_DepthTexture, &view_desc, &m_DepthStencilShaderView);
		api->HandleErrors(hr, "[Texture](Initiate) : Failed to create DepthStencil-ShaderResourceView.");
		api->SetDebugName(m_DepthStencilShaderView, debug_name + "DepthStencil-ShaderResourceView");
	}

	if (flags & D3D11_BIND_DEPTH_STENCIL)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depth_desc;
		ZeroMemory(&depth_desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		depth_desc.Format = depth_stencil_format;
		depth_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depth_desc.Texture2D.MipSlice = 0;

		hr = device->CreateDepthStencilView(m_DepthTexture, &depth_desc, &m_DepthStencilView);
		api->HandleErrors(hr, "[Texture](Initiate) : Failed to create Depthstencil-DepthStencilView.");
		api->SetDebugName(m_DepthStencilView, debug_name + "DepthStencilView");
	}

}

void Texture::InitiateAsDepthStencil(float width, float height, const std::string& debug_name)
{
	Initiate(width, height
		, 0//DEFAULT_USAGE | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL// flags
		, graphics::R32_TYPELESS
		, graphics::R32_FLOAT
		, graphics::DEPTH_32_FLOAT
		, debug_name);
}

void Texture::InitiateAsRenderTarget(float width, float height, const std::string& debug_name)
{
	Initiate(width, height,
		DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R16G16B16A16_FLOAT
		, debug_name);
}

void Texture::InitiateWithColor(cl::SColor color)
{
	//Initiate(64.f, 64.f, DEFAULT_USAGE | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R8G8B8A8_UNORM, "Colored_Texture");
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &color._color;
	data.SysMemPitch = 64.f * 4;

	D3D11_TEXTURE2D_DESC info;
	info.Width = 64.f;
	info.Height = 64.f;
	info.MipLevels = 1;
	info.ArraySize = 1;
	info.SampleDesc.Count = 1;
	info.SampleDesc.Quality = 0;
	info.MiscFlags = 0;
	info.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	info.Usage = D3D11_USAGE_DYNAMIC;
	info.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	info.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ID3D11Texture2D* texture;
	Engine::GetAPI()->GetDevice()->CreateTexture2D(&info, &data, &texture);
	DL_ASSERT_EXP(texture != nullptr, "Texture is nullptr!");
	Engine::GetAPI()->GetDevice()->CreateShaderResourceView(texture, nullptr, &m_ShaderResource);
	texture->Release();
}

void Texture::Initiate3DTexture(u16 width, u16 height, u16 depth, graphics::eTextureFormat texture_format, const std::string& debug_name)
{
	DirectX11* api = Engine::GetInstance()->GetAPI();
	ID3D11Texture3D* texture;
	D3D11_TEXTURE3D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.Depth = depth;
	desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE ;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.MiscFlags = 0;
	
	api->GetDevice()->CreateTexture3D(&desc, nullptr , &texture);
	api->SetDebugName(texture, debug_name);


	{
		D3D11_TEXTURE3D_DESC rtdesc;
		rtdesc.Width = ( UINT ) width;
		rtdesc.Height = ( UINT ) height;
		rtdesc.Depth = ( UINT )depth;
		rtdesc.MipLevels = 1;
		rtdesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		rtdesc.Usage = D3D11_USAGE_DEFAULT;
		rtdesc.BindFlags = D3D11_BIND_RENDER_TARGET;
		rtdesc.CPUAccessFlags = 0;
		rtdesc.MiscFlags = 0;

		ID3D11Texture3D* _texture = nullptr;
		HRESULT hr = api->GetDevice()->CreateTexture3D(&rtdesc, NULL, &_texture);
		api->HandleErrors(hr, "[Texture](Initiate) : Failed to initiate texture.");

		hr = api->GetDevice()->CreateRenderTargetView(_texture, NULL, &m_RenderTargetView);
		api->HandleErrors(hr, "[Texture](Initiate) : Failed to create RenderTargetView.");
		api->SetDebugName(m_RenderTargetView, debug_name + "RenderTargetView");
		SAFE_RELEASE(_texture);
	}

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
		ZeroMemory(&view_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		view_desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		view_desc.Texture3D.MipLevels = 1;
		view_desc.Texture3D.MostDetailedMip = 0;

		HRESULT hr = api->GetDevice()->CreateShaderResourceView(texture, &view_desc, &m_ShaderResource);
		api->HandleErrors(hr, "[Texture](Initiate) : Failed to create DepthStencil-ShaderResourceView.");
		api->SetDebugName(m_ShaderResource, debug_name + "DepthStencil-ShaderResourceView");
	}

	SAFE_RELEASE(texture);

}

void Texture::Initiate3DTexture(u16 width, u16 height, u16 depth, graphics::eTextureFormat texture_format, s32 layer_count, const std::string& debug_name)
{
	DirectX11* api = Engine::GetInstance()->GetAPI();

	D3D11_TEXTURE2D_DESC desc3d;
	desc3d.Width = ( UINT ) width;
	desc3d.Height = ( UINT ) height;
	//desc3d.Depth = ( UINT ) depth;
	desc3d.ArraySize = layer_count;
	desc3d.MipLevels = 1;
	desc3d.Format = texture_format;
	desc3d.Usage = D3D11_USAGE_DEFAULT;
	desc3d.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc3d.CPUAccessFlags = 0;
	desc3d.SampleDesc.Count = 1;
	desc3d.SampleDesc.Quality = 0;
	desc3d.MiscFlags = 0;

	ID3D11Texture2D* _texture = nullptr;
	HRESULT hr = api->GetDevice()->CreateTexture2D(&desc3d, NULL, &_texture);
	api->HandleErrors(hr, "[Texture](Initiate) : Failed to initiate texture.");
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtdesc;
		rtdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtdesc.Format = texture_format;
		rtdesc.Texture2DArray.ArraySize = depth;
		rtdesc.Texture2DArray.FirstArraySlice = 1;
		rtdesc.Texture2DArray.MipSlice = 0;

		hr = api->GetDevice()->CreateRenderTargetView(_texture, &rtdesc, &m_RenderTargetView);
		api->HandleErrors(hr, "[Texture](Initiate) : Failed to create RenderTargetView.");
		api->SetDebugName(m_RenderTargetView, debug_name + "RenderTargetView");
	}

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
		ZeroMemory(&view_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		view_desc.Format = texture_format;
		view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		view_desc.Texture2D.MipLevels = 1;
		view_desc.Texture2D.MostDetailedMip = 0;

		hr = api->GetDevice()->CreateShaderResourceView(_texture, &view_desc, &m_ShaderResource);
		api->HandleErrors(hr, "[Texture](Initiate) : Failed to create DepthStencil-ShaderResourceView.");
		api->SetDebugName(m_ShaderResource, debug_name + "DepthStencil-ShaderResourceView");
		
	}
	SAFE_RELEASE(_texture);
}

// UsageType Texture::GetUsage(int flags) const
// {
// 	if (flags & IMMUTABLE_USAGE)
// 	{
// 		return D3D11_USAGE_IMMUTABLE;
// 	}
// 	else if (flags & STAGING_USAGE)
// 	{
// 		return D3D11_USAGE_STAGING;
// 	}
// 	else if (flags & DYNAMIC_USAGE)
// 	{
// 		return D3D11_USAGE_DYNAMIC;
// 	}
// 	return D3D11_USAGE_DEFAULT;
// }

// bool Texture::Load(std::string filepath)
// {
// 	DL_MESSAGE("Loading texture : %s", filepath.c_str());
// 	myFileName = filepath;
// 	ID3D11Device* device = Engine::GetAPI()->GetDevice();
// 
// 		/*	
// 		HRESULT hr = DirectX::CreateDDSTextureFromFileEx(
// 		device, 
// 		std::wstring(filepath.begin(), filepath.end()).c_str(), 
// 		0, 
// 		D3D11_USAGE_DEFAULT, 
// 		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, 
// 		D3D11_CPU_ACCESS_READ, 
// 		D3D11_RESOURCE_MISC_GENERATE_MIPS, 
// 		false, 
// 		nullptr, 
// 		&m_ShaderResource);
// 		*/
// 
// 	HRESULT hr = DirectX::CreateDDSTextureFromFile(device
// 		, nullptr
// 		, std::wstring(filepath.begin(), filepath.end()).c_str()
// 		, nullptr
// 		, &m_ShaderResource);
// 
// 
// 	std::string debugname = cl::substr(filepath, "/", false, 0);
// 	Engine::GetAPI()->SetDebugName(m_ShaderResource, debugname);
// 	DL_MESSAGE_EXP(FAILED(hr), "[Texture](Load) : Failed to load texture %s", filepath.c_str());
// 	if ( FAILED(hr) )
// 	{
// 		DL_ASSERT("Failed to load texture!");
// 		return false;
// 	}
// 	ID3D11Resource* pResource = nullptr;
// 	m_ShaderResource->GetResource(&pResource);
// 	pResource->QueryInterface<ID3D11Texture2D>(&m_Texture);
// 	if (m_Texture)
// 	{
// 		D3D11_TEXTURE2D_DESC desc;
// 		m_Texture->GetDesc(&desc);
// 		myHeight = desc.Height;
// 		myWidth = desc.Width;
// 	}
// 	else
// 		DL_MESSAGE("m_Texture was null while loading, this is fine for volume textures");
// 	//m_ShaderResource->GetResource(&desc);
// 	
// 
// 	return true;
// }

void Texture::OnReload()
{
	this->~Texture();
	Load(myFileName);
}

void Texture::SetTexture(IShaderResourceView* aShaderResource)
{
	m_ShaderResource = aShaderResource;
}

// HRESULT Texture::SaveToFile(ITexture2D*& texture_resource, const std::string& file_name)
// {
// 	ID3D11Resource* resource = nullptr;
// 	HRESULT hr = texture_resource->QueryInterface(IID_ID3D11Texture2D, (void**)&resource);
// 	Engine::GetAPI()->HandleErrors(hr, "Failed to query interface of texture_resource");
// 	std::wstring middle_hand(file_name.begin(), file_name.end());
// 	LPCWSTR new_name(middle_hand.c_str());
// 	hr = DirectX::SaveDDSTextureToFile(Engine::GetAPI()->GetContext(), resource, new_name);
// 	resource->Release();
// 	resource = nullptr;
// 	return S_OK;
// }
// 
// void Texture::Save2DResource(const std::string& filename)
// {
// 	ID3D11Resource* resource = nullptr;
// 	HRESULT hr = m_ShaderResource->QueryInterface(IID_ID3D11Texture2D, ( void** ) & resource);
// 	Engine::GetAPI()->HandleErrors(hr, "failed to query interface of 3dtexture");
// 	std::wstring mid(filename.begin(), filename.end());
// 	LPCWSTR new_name(mid.c_str());
// 	DirectX::SaveDDSTextureToFile(Engine::GetAPI()->GetContext(), resource, new_name);
// 	resource->Release();
// 	resource = nullptr;
// }
// 
// void Texture::Save3DResource(const std::string& filename)
// {
// 	return;
// 	ID3D11Resource* resource = nullptr;
// 	HRESULT hr = m_ShaderResource->QueryInterface(IID_ID3D11Texture3D, ( void** ) & resource);
// 	Engine::GetAPI()->HandleErrors(hr, "failed to query interface of 3dtexture");
// 	std::wstring mid(filename.begin(), filename.end());
// 	LPCWSTR new_name(mid.c_str());
// 	DirectX::SaveDDSTextureToFile(Engine::GetAPI()->GetContext(), resource, new_name);
// 	resource->Release();
// 	resource = nullptr;
// }

// void Texture::CopyData(ITexture2D* dest, ITexture2D* source)
// {
// 	Engine::GetGraphicsAPI()->CopyResource(dest, source);
// }
