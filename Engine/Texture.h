#pragma once
#pragma message ("Compiling File : Texture.h")
#include "engine_shared.h"

enum TextureUsageFlags
{
	DEFAULT_USAGE = 2048,
	IMMUTABLE_USAGE = 4096,
	DYNAMIC_USAGE = 8192,
	STAGING_USAGE = 16384
};

#include <Utilities.h>
class Texture
{
public:
	Texture() = default;

	void Initiate(u16 width, u16 height, s32 flags, TextureFormat texture_format, const std::string& debug_name);
	void Initiate(u16 width, u16 height, s32 flags, TextureFormat texture_format, TextureFormat shader_resource_view_format, TextureFormat depth_stencil_format, const std::string& debug_name);
	void Initiate(u16 width, u16 height, s32 flags, TextureFormat render_target_format, TextureFormat texture_format, TextureFormat shader_resource_view_format, TextureFormat depth_stencil_format, const std::string& debug_name);

	void InitiateAsDepthStencil(float width, float height, const std::string& debug_name);
	void InitiateAsRenderTarget(float width, float height, const std::string& debug_name);

	void InitiateWithColor(CL::SColor color);

	void Initiate3DTexture(u16 width, u16 height, u16 depth, TextureFormat texture_format, const std::string& debug_name);
	void Initiate3DTexture(u16 width, u16 height, u16 depth, TextureFormat texture_format, s32 layer_count, const std::string& debug_name);

	void Save2DResource(const std::string& filename);
	void Save3DResource(const std::string& filename);
	bool CleanUp();
	bool Load(std::string filepath);
	void OnReload();
#ifdef SNOWBLIND_DX11
	Ticket_Mutex m_Mutex;
	ITexture2D* GetDepthTexture() const { return m_DepthTexture; }
	ITexture2D* GetTexture() const { return m_DepthTexture; }
	IShaderResourceView* GetShaderView() { return m_ShaderResource; }
	IShaderResourceView** GetShaderViewRef() { return &m_ShaderResource; }


	IShaderResourceView* GetDepthStencilView() const { return m_DepthStencilShaderView; }
	IShaderResourceView** GetDSVResourceRef() { return &m_DepthStencilShaderView; }
	IRenderTargetView*	GetRenderTargetView() const { return m_RenderTargetView; }
	IRenderTargetView** GetRenderTargetRef() { return &m_RenderTargetView; }

	IDepthStencilView* GetDepthView() const { return m_DepthStencilView; }


	void SetTexture(IShaderResourceView* aShaderResource);
	static HRESULT SaveToFile(ITexture2D*& texture_resource, const std::string& file_name);
	static void CopyData(ITexture2D* dest, ITexture2D* source);
#endif

	s32 GetWidth() const { return myWidth; }
	s32 GetHeight() const { return myHeight; }
	//void CreateDepthStencilView(float aWidth, float aHeight, int aArraySize = 1);
private:

	UsageType GetUsage(int flags) const;

	ITexture2D* m_DepthTexture = nullptr;


	union
	{
		IShaderResourceView* m_ShaderResource;
		IShaderResourceView* m_DepthStencilShaderView;
	};

	IDepthStencilView* m_DepthStencilView = nullptr;
	IRenderTargetView* m_RenderTargetView = nullptr;
	s32	myWidth;
	s32	myHeight;

	std::string myFileName;
};
