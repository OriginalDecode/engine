#include "stdafx.h"
#include "Texture.h"
#include <Engine/IGraphicsDevice.h>
#include <Engine/Engine.h>

Texture::Texture(IShaderResourceView* srv)
	: m_ShaderResource(srv)
{
}

Texture::~Texture()
{
	graphics::IGraphicsAPI* api = Engine::GetAPI();
	api->ReleasePtr(m_ShaderResource);
	api->ReleasePtr(m_DepthTexture);
	api->ReleasePtr(m_DepthStencilView);
	api->ReleasePtr(m_DepthStencilShaderView);
	api->ReleasePtr(m_RenderTargetView);
}

void Texture::Initiate(const TextureDesc& desc, const cl::HashString& debug_name)
{
	Initiate(desc, true, debug_name);
}

void Texture::Initiate(const TextureDesc& desc, bool create_from_texture, const cl::HashString& debug_name)
{
	graphics::IGraphicsDevice& device = Engine::GetAPI()->GetDevice();
	DL_ASSERT_EXP(desc.m_TextureFormat != graphics::NO_FORMAT, "invalid texture format!");
	if (desc.m_TextureFormat != graphics::NO_FORMAT)
	{
		graphics::Texture2DDesc tex_desc;
		tex_desc.m_Height = desc.m_Height;
		tex_desc.m_Width = desc.m_Width;
		tex_desc.m_Format = desc.m_TextureFormat;
		tex_desc.m_MipLevels = 0;
		tex_desc.m_ArraySize = 0;
		tex_desc.m_Usage = desc.m_Usage;
		tex_desc.m_MiscFlags = 0;
		tex_desc.m_SampleCount = 1;
		tex_desc.m_SampleQuality = 0;
		tex_desc.m_CPUAccessFlag = desc.m_CPUAccessFlag;
		tex_desc.m_Binding = desc.m_ResourceTypeBinding;
		graphics::IGraphicsDevice& device = Engine::GetAPI()->GetDevice();
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

void Texture::InitiateAsDepthStencil(float width, float height, const cl::HashString& debug_name)
{
	TextureDesc desc;
	desc.m_Width = (s32)width;
	desc.m_Height = (s32)height;
	desc.m_ResourceTypeBinding = graphics::BIND_SHADER_RESOURCE | graphics::BIND_DEPTH_STENCIL;
	desc.m_Usage = graphics::DEFAULT_USAGE;
	desc.m_TextureFormat = graphics::R32_TYPELESS;
	desc.m_DepthTextureFormat = graphics::DEPTH_32_FLOAT;
	//desc.m_RenderTargetFormat = graphics::R32_TYPELESS;
	desc.m_ShaderResourceFormat = graphics::R32_FLOAT;

	Initiate(desc, debug_name);
}

void Texture::InitiateAsRenderTarget(float width, float height, const cl::HashString& debug_name)
{
	TextureDesc desc;
	desc.m_Width = (s32)width;
	desc.m_Height = (s32)height;
	desc.m_ResourceTypeBinding = graphics::BIND_SHADER_RESOURCE | graphics::BIND_RENDER_TARGET;
	desc.m_Usage = graphics::DEFAULT_USAGE;
	desc.m_TextureFormat = graphics::RGBA16_FLOAT;
	desc.m_RenderTargetFormat = graphics::RGBA16_FLOAT;

	Initiate(desc, debug_name);

}
