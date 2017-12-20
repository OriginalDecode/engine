#include "stdafx.h"
#include "Texture.h"
#include <Engine/IGraphicsDevice.h>
#include <Engine/Engine.h>
#include <CommonLib/reflector.h>
Texture::Texture(IShaderResourceView* srv)
	: m_ShaderResource(srv)
{
}

Texture::~Texture()
{
	graphics::IGraphicsAPI* api = Engine::GetAPI();
	api->ReleasePtr(m_ShaderResource);
	m_ShaderResource = nullptr;
	api->ReleasePtr(m_DepthTexture);
	m_DepthTexture = nullptr;
	api->ReleasePtr(m_DepthStencilView);
	m_DepthStencilView = nullptr;
	api->ReleasePtr(m_RenderTargetView);
	m_RenderTargetView = nullptr;
}

void Texture::Initiate(const TextureDesc& desc, const std::string& debug_name)
{
	Initiate(desc, true, debug_name);
}

void Texture::Initiate(const TextureDesc& desc, bool create_from_texture, const std::string& debug_name)
{
#ifdef _DEBUG
	m_DebugName = debug_name;
#endif
	graphics::IGraphicsDevice& device = Engine::GetAPI()->GetDevice();
	DL_ASSERT_EXP(desc.m_TextureFormat != graphics::NO_FORMAT, "invalid texture format!");
	if (desc.m_TextureFormat != graphics::NO_FORMAT)
	{
		graphics::Texture2DDesc tex_desc;
		tex_desc.m_Height = desc.m_Height;
		tex_desc.m_Width = desc.m_Width;
		tex_desc.m_Format = desc.m_TextureFormat;
		tex_desc.m_MipLevels = 1;
		tex_desc.m_ArraySize = 1;
		tex_desc.m_Usage = desc.m_Usage;
		tex_desc.m_MiscFlags = 0;
		tex_desc.m_SampleCount = 1;
		tex_desc.m_SampleQuality = 0;
		tex_desc.m_CPUAccessFlag = 0;
		tex_desc.m_Binding = desc.m_ResourceTypeBinding;
		m_Texture = device.CreateTexture2D(tex_desc, debug_name + "_Texture");
	}

	if (desc.m_ShaderResourceFormat != graphics::NO_FORMAT)
	{
		graphics::Texture2DDesc tex_desc;
		tex_desc.m_Height = desc.m_Height;
		tex_desc.m_Width = desc.m_Width;
		tex_desc.m_Format = desc.m_ShaderResourceFormat;
		tex_desc.m_MipLevels = 0;
		tex_desc.m_ArraySize = 0;
		tex_desc.m_Usage = desc.m_Usage;
		tex_desc.m_MiscFlags = 0;
		tex_desc.m_SampleCount = 1;
		tex_desc.m_SampleQuality = 0;
		tex_desc.m_CPUAccessFlag = desc.m_CPUAccessFlag;
		tex_desc.m_Binding = desc.m_ResourceTypeBinding;
		if (create_from_texture)
			m_ShaderResource = device.CreateShaderResource(tex_desc, m_Texture, debug_name + "_SRV");
		else
			m_ShaderResource = device.CreateShaderResource(m_Texture, debug_name + "_SRV");

	}

	if (desc.m_RenderTargetFormat != graphics::NO_FORMAT)
	{
		graphics::Texture2DDesc tex_desc;
		tex_desc.m_Height = desc.m_Height;
		tex_desc.m_Width = desc.m_Width;
		tex_desc.m_Format = desc.m_RenderTargetFormat;
		tex_desc.m_MipLevels = 0;
		tex_desc.m_ArraySize = 0;
		tex_desc.m_Usage = desc.m_Usage;
		tex_desc.m_MiscFlags = 0;
		tex_desc.m_SampleCount = 1;
		tex_desc.m_SampleQuality = 0;
		tex_desc.m_CPUAccessFlag = desc.m_CPUAccessFlag;
		tex_desc.m_Binding = graphics::BIND_RENDER_TARGET;
		m_RenderTargetView = device.CreateRenderTarget(tex_desc, m_Texture, debug_name + "_RTV");
	}

	if (desc.m_DepthTextureFormat != graphics::NO_FORMAT)
	{
		graphics::Texture2DDesc tex_desc;
		tex_desc.m_Height = desc.m_Height;
		tex_desc.m_Width = desc.m_Width;
		tex_desc.m_Format = desc.m_DepthTextureFormat;
		tex_desc.m_MipLevels = 0;
		tex_desc.m_ArraySize = 0;
		tex_desc.m_Usage = desc.m_Usage;
		tex_desc.m_MiscFlags = 0;
		tex_desc.m_SampleCount = 1;
		tex_desc.m_SampleQuality = 0;
		tex_desc.m_CPUAccessFlag = desc.m_CPUAccessFlag;
		tex_desc.m_Binding = graphics::BIND_DEPTH_STENCIL;
		m_DepthStencilView = device.CreateDepthStencilView(tex_desc, m_Texture, debug_name + "_DSV");
	}
}

void Texture::InitiateAsDepthStencil(s32 width, s32 height, const std::string& debug_name)
{
	TextureDesc desc;
	desc.m_Width = width;
	desc.m_Height = height;
	desc.m_ResourceTypeBinding = graphics::BIND_SHADER_RESOURCE | graphics::BIND_DEPTH_STENCIL;
	desc.m_Usage = graphics::DEFAULT_USAGE;
	desc.m_TextureFormat = graphics::R32_TYPELESS;
	desc.m_DepthTextureFormat = graphics::DEPTH_32_FLOAT;
	//desc.m_RenderTargetFormat = graphics::R32_TYPELESS;
	desc.m_ShaderResourceFormat = graphics::R32_FLOAT;

	Initiate(desc, debug_name);
}

void Texture::InitiateAsRenderTarget(s32 width, s32 height, const std::string& debug_name)
{
	TextureDesc desc;
	desc.m_Width = width;
	desc.m_Height = height;
	desc.m_ResourceTypeBinding = graphics::BIND_SHADER_RESOURCE | graphics::BIND_RENDER_TARGET;
	desc.m_Usage = graphics::DEFAULT_USAGE;
	desc.m_TextureFormat = graphics::RGBA16_FLOAT;
	desc.m_ShaderResourceFormat = graphics::RGBA16_FLOAT;
	desc.m_RenderTargetFormat = graphics::RGBA16_FLOAT;

	Initiate(desc, debug_name);

}

void Texture::InitiateTextureArray(const char paths[], const char* debug_name)
{
	D3D11_TEXTURE2D_DESC desc;
	const u32 tex_count = ARRSIZE(paths);

	ID3D11Resource* src[tex_count];

	((ID3D11Texture2D*)src[0])->GetDesc(&desc);
	
	/* https://stackoverflow.com/questions/19364012/d3d11-creating-a-cube-map-from-6-images */

	D3D11_TEXTURE2D_DESC arr_desc;
	arr_desc.Width = 0;
	arr_desc.Height = 0;
	arr_desc.MipLevels = 0;
	arr_desc.ArraySize = tex_count;
	arr_desc.Format = desc.Format;
	arr_desc.SampleDesc.Count = 1;
	arr_desc.SampleDesc.Quality = 0;
	arr_desc.Usage = D3D11_USAGE_DEFAULT;
	arr_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	arr_desc.CPUAccessFlags = 0;
	arr_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	



}
