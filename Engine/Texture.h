#pragma once
#pragma message ("Compiling File : Texture.h")
#include "engine_shared.h"
#include <Utilities.h>


class Texture
{
public:
	Texture() = default;
	~Texture();
	void Initiate(u16 width, u16 height, s32 usage, s32 binding, graphics::eTextureFormat texture_format, const std::string& debug_name);
	void Initiate(u16 width, u16 height, s32 flags, graphics::eTextureFormat texture_format, graphics::eTextureFormat shader_resource_view_format, graphics::eTextureFormat depth_stencil_format, const std::string& debug_name);
	void Initiate(u16 width, u16 height, s32 flags, graphics::eTextureFormat render_target_format, graphics::eTextureFormat texture_format, graphics::eTextureFormat shader_resource_view_format, graphics::eTextureFormat depth_stencil_format, const std::string& debug_name);

	void InitiateAsDepthStencil(float width, float height, const std::string& debug_name);
	void InitiateAsRenderTarget(float width, float height, const std::string& debug_name);

	void InitiateWithColor(cl::SColor color);

	void Initiate3DTexture(u16 width, u16 height, u16 depth, graphics::eTextureFormat texture_format, const std::string& debug_name);
	void Initiate3DTexture(u16 width, u16 height, u16 depth, graphics::eTextureFormat texture_format, s32 layer_count, const std::string& debug_name);

	void Save2DResource(const std::string& filename);
	void Save3DResource(const std::string& filename);
	bool Load(std::string filepath);
	void OnReload();
	Ticket_Mutex m_Mutex;
	ITexture2D* GetDepthTexture() const { return m_DepthTexture; }
	ITexture2D* GetTexture() const { return m_Texture; }
	void* GetShaderView() { return m_ShaderResource; }
	IShaderResourceView** GetShaderViewRef() { return &m_ShaderResource; }


	IShaderResourceView* GetDepthStencilView() const { return m_DepthStencilShaderView; }
	IShaderResourceView** GetDSVResourceRef() { return &m_DepthStencilShaderView; }
	IRenderTargetView*	GetRenderTargetView() const { return m_RenderTargetView; }
	IRenderTargetView** GetRenderTargetRef() { return &m_RenderTargetView; }

	IDepthStencilView* GetDepthView() const { return m_DepthStencilView; }


	void SetTexture(IShaderResourceView* aShaderResource);
	//static HRESULT SaveToFile(ITexture2D*& texture_resource, const std::string& file_name);
	//static void CopyData(ITexture2D* dest, ITexture2D* source);

	s32 GetWidth() const { return m_Width; }
	s32 GetHeight() const { return m_Height; }
	//void CreateDepthStencilView(float aWidth, float aHeight, int aArraySize = 1);
private:

	//UsageType GetUsage(int flags) const;

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
