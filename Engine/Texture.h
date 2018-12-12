#pragma once
#pragma message ("Compiling File : Texture.h")
#include "engine_shared.h"
#include <Utilities.h>
#include <Engine/IGraphicsAPI.h>

namespace texture_format
{
	enum FORMAT
	{
		DDS,
		PNG,
		JPG,
		TGA,
	};
};
struct TextureDesc
{
	int32 m_Width = 0;
	int32 m_Height = 0;
	graphics::eTextureFormat m_TextureFormat = graphics::NO_FORMAT;
	graphics::eTextureFormat m_ShaderResourceFormat = graphics::NO_FORMAT;
	graphics::eTextureFormat m_RenderTargetFormat = graphics::NO_FORMAT;
	graphics::eTextureFormat m_DepthTextureFormat = graphics::NO_FORMAT;
	int32 m_ResourceTypeBinding = 0;
	int32 m_Usage = 0;
	int32 m_CPUAccessFlag = 0;
	int32 m_MiscFlags = 0;
	int32 m_MipCount = 1;
};

class Texture
{
public:
	Texture() = default;
	Texture(IShaderResourceView* srv);
	Texture(IShaderResourceView* srv, ITexture2D* tex);
#ifdef _DEBUG
	Texture(IShaderResourceView* srv, std::string debug_name);
	Texture(IShaderResourceView* srv, ITexture2D* tex, std::string debug_name);
#endif
	~Texture();


	void Initiate(const TextureDesc& desc, const std::string& debug_name);
	void Initiate(const TextureDesc& desc, bool create_from_texture, const std::string& debug_name);


	void InitiateAsDepthStencil(int32 width, int32 height, const std::string& debug_name);
	void InitiateAsRenderTarget(int32 width, int32 height, const std::string& debug_name);

	//Used for cubemaps or 3d textures
	void CreateTextureArray(const char* paths[], const int32 num_tex, const char* filename);
	void CreateTextureArray(Texture* textures[], const int32 num_tex, const char* filename);
	void Create3DTexture(const char* path, int32 slice_width, int32 slice_height, int32 slice_count, const char* filename);
	//void Create3DTexture(Texture* textures[], s32 slice_width, s32 slice_height, s32 slice_count, const char* filename);

	static void SaveToDisk(const wchar_t* path, ITexture2D* tex, texture_format::FORMAT format = texture_format::DDS);


	ITexture2D* GetDepthTexture() const { return m_DepthTexture; }
	ITexture2D* GetTexture() const { return m_Texture; }
	ITexture2D* GetTexture() { return m_Texture; }
	IShaderResourceView* GetShaderView() { return m_ShaderResource; }
	IShaderResourceView** GetShaderViewRef() { return &m_ShaderResource; }
 	IShaderResourceView* GetDepthStencilView() const { return m_DepthStencilShaderView; }
	IShaderResourceView** GetDSVResourceRef() { return &m_DepthStencilShaderView; }
	IRenderTargetView*	GetRenderTargetView() const { return m_RenderTargetView; }
	IRenderTargetView** GetRenderTargetRef() { return &m_RenderTargetView; }
	IDepthStencilView* GetDepthView() const { return m_DepthStencilView; }

	int32 GetWidth() const { return m_Width; }
	int32 GetHeight() const { return m_Height; }

#ifdef _DEBUG
	std::string GetDebugName() { return m_DebugName; }
#endif
private:
#ifdef _DEBUG
	std::string m_DebugName;
#endif
	union
	{
		ITexture2D* m_DepthTexture = nullptr;
		ITexture2D* m_Texture;
	};

	union
	{
		IShaderResourceView* m_ShaderResource = nullptr;
		IShaderResourceView* m_DepthStencilShaderView;
	};
	IDepthStencilView* m_DepthStencilView = nullptr;
	IRenderTargetView* m_RenderTargetView = nullptr;
	int32	m_Width;
	int32	m_Height;

};
