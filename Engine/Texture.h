#pragma once
#pragma message ("Compiling File : Texture.h")
#include "engine_shared.h"
#include <Utilities.h>
#include <Engine/IGraphicsAPI.h>
struct TextureDesc
{
	s32 m_Width = 0;
	s32 m_Height = 0;
	graphics::eTextureFormat m_TextureFormat = graphics::NO_FORMAT;
	graphics::eTextureFormat m_ShaderResourceFormat = graphics::NO_FORMAT;
	graphics::eTextureFormat m_RenderTargetFormat = graphics::NO_FORMAT;
	graphics::eTextureFormat m_DepthTextureFormat = graphics::NO_FORMAT;
	s32 m_ResourceTypeBinding = 0;
	s32 m_Usage = 0;
	s32 m_CPUAccessFlag = 0;
};

class Texture
{
public:
	Texture() = default;
	~Texture();


	void Initiate(const TextureDesc& desc, const cl::HashString& debug_name);
	void InitiateAsDepthStencil(float width, float height, const cl::HashString& debug_name);
	void InitiateAsRenderTarget(float width, float height, const cl::HashString& debug_name);


	ITexture2D* GetDepthTexture() const { return m_DepthTexture; }
	ITexture2D* GetTexture() const { return m_Texture; }
	IShaderResourceView* GetShaderView() { return m_ShaderResource; }
	IShaderResourceView** GetShaderViewRef() { return &m_ShaderResource; }
	IShaderResourceView* GetDepthStencilView() const { return m_DepthStencilShaderView; }
	IShaderResourceView** GetDSVResourceRef() { return &m_DepthStencilShaderView; }
	IRenderTargetView*	GetRenderTargetView() const { return m_RenderTargetView; }
	IRenderTargetView** GetRenderTargetRef() { return &m_RenderTargetView; }
	IDepthStencilView* GetDepthView() const { return m_DepthStencilView; }

	s32 GetWidth() const { return m_Width; }
	s32 GetHeight() const { return m_Height; }


private:
	union
	{
		ITexture2D* m_DepthTexture = nullptr;
		ITexture2D* m_Texture;
	};
	union
	{
		IShaderResourceView* m_ShaderResource;
		IShaderResourceView* m_DepthStencilShaderView;
	};
	IDepthStencilView* m_DepthStencilView = nullptr;
	IRenderTargetView* m_RenderTargetView = nullptr;
	s32	m_Width;
	s32	m_Height;

};
