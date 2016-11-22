#pragma once
#pragma message ("Compiling File : Texture.h")

#include "snowblind_shared.h"


enum TextureUsageFlags
{
	DEFAULT_USAGE = 2048,
	IMMUTABLE_USAGE = 4096,
	DYNAMIC_USAGE = 8192,
	STAGING_USAGE = 16384
};

namespace Snowblind
{
	class CTexture
	{
	public:
		CTexture() = default;

		void Initiate(u16 width, u16 height, s32 flags, TextureFormat texture_format, const std::string& debug_name);
		void Initiate(u16 width, u16 height, s32 flags, TextureFormat texture_format
			, TextureFormat shader_resource_view_format, TextureFormat depth_stencil_format, const std::string& debug_name);
		void Initiate(u16 width, u16 height, s32 flags, TextureFormat render_target_format, TextureFormat texture_format, TextureFormat shader_resource_view_format, TextureFormat depth_stencil_format, const std::string& debug_name);

		bool CleanUp();
		bool Load(const std::string& filepath);

#ifdef SNOWBLIND_DX11

		ITexture2D* GetDepthTexture() const { return m_DepthTexture; }
		IShaderResourceView* GetShaderView() const { return m_ShaderResource; }
		IShaderResourceView* GetDepthStencilView() const { return m_DepthStencilShaderView; }
		IRenderTargetView*	GetRenderTargetView() const { return m_RenderTargetView; }
		IDepthStencilView* GetDepthView() const { return m_DepthStencilView; }

		void SetTexture(IShaderResourceView* aShaderResource);
		static HRESULT SaveToFile(ITexture2D*& texture_resource, const std::string& file_name);
		static void CopyData(ITexture2D* dest, ITexture2D* source);
#endif

		//void CreateDepthStencilView(float aWidth, float aHeight, int aArraySize = 1);
	private:

		UsageType GetUsage(int flags) const;

#ifdef SNOWBLIND_DX11
		ITexture2D* m_DepthTexture = nullptr;

		IShaderResourceView* m_ShaderResource = nullptr;
		IShaderResourceView* m_DepthStencilShaderView = nullptr;

		IDepthStencilView* m_DepthStencilView = nullptr;
		IRenderTargetView* m_RenderTargetView = nullptr;
#endif
		s32	myWidth;
		s32	myHeight;

		std::string myFileName;
	};
};