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

void Texture::Initiate(const TextureDesc& desc, const cl::CHashString<128>& debug_name)
{
	graphics::IGraphicsDevice& device = Engine::GetAPI()->GetDevice();
	if (desc.m_TextureFormat != graphics::NO_FORMAT)
	{
		graphics::Texture2DDesc tex_desc;
		tex_desc.m_Height = m_Height;
		tex_desc.m_Width = m_Width;
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
		graphics::ShaderResourceDesc srv_desc;
		m_ShaderResource = device.CreateShaderResource(srv_desc, m_Texture, debug_name + "_SRV");
	}

	if (desc.m_RenderTargetFormat != graphics::NO_FORMAT)
	{
		graphics::RenderTargetDesc rtv_desc;
		m_RenderTargetView = device.CreateRenderTarget(rtv_desc, m_Texture, debug_name + "_RTV");
	}

	if (desc.m_DepthTextureFormat != graphics::NO_FORMAT)
	{
		graphics::DepthDesc depth_desc;
		m_DepthStencilView = device.CreateDepthStencilView(depth_desc, m_Texture, debug_name + "_DSV");
	}
}

void Texture::InitiateAsDepthStencil(float width, float height, const cl::CHashString<128>& debug_name)
{
	TextureDesc desc;
	desc.m_Width = (s32)width;
	desc.m_Height = (s32)height;
	desc.m_ResourceTypeBinding = graphics::SHADERRESOURCE | graphics::DEPTHSTENCIL;
	desc.m_Usage = graphics::DEFAULT_USAGE;
	desc.m_DepthTextureFormat = graphics::DEPTH_32_FLOAT;
	desc.m_RenderTargetFormat = graphics::R32_TYPELESS;
	desc.m_ShaderResourceFormat = graphics::R32_FLOAT;

	Initiate(desc, debug_name);
}

void Texture::InitiateAsRenderTarget(float width, float height, const cl::CHashString<128>& debug_name)
{
	TextureDesc desc;
	desc.m_Width = (s32)width;
	desc.m_Height = (s32)height;
	desc.m_ResourceTypeBinding = graphics::SHADERRESOURCE | graphics::RENDERTARGET;
	desc.m_Usage = graphics::DEFAULT_USAGE;
	desc.m_RenderTargetFormat = graphics::RGBA16_FLOAT;

	Initiate(desc, debug_name);

}
