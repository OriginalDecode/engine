#pragma once
#pragma message ("Texture.h is being compiled")
#ifdef SNOWBLIND_DX11
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
#endif
typedef DXGI_FORMAT TextureFormat;

enum TextureUsageFlags
{
	DEFAULT_USAGE,
	IMMUTABLE_USAGE,
	DYNAMIC_USAGE,
	STAGING_USAGE,
};

namespace Snowblind
{
	class CTexture
	{
	public:
		CTexture() = default;
		CTexture(float aWidth, float aHeight, unsigned int aBindFlag, unsigned int aFormat);

		bool CleanUp();

		void InitAsDepthBuffer(float aWidth, float aHeight);
		void InitStencil(float aWidth, float aHeight);

		void Initiate(u16 width, u16 height, s32 flags, TextureFormat format, const std::string& debug_name);
		void Initiate(u16 width, u16 height, s32 flags, TextureFormat format, TextureFormat depth_view_format, TextureFormat depth_stencil_format, const std::string& debug_name);


		const std::string& GetFileName();
		bool LoadTexture(const std::string& aFileName, bool mip = false);

		void SetDebugName(const std::string& debugName);
		void CreateTexture(const std::string& aTexturePath);
#ifdef SNOWBLIND_DX11
		ID3D11Texture2D* GetDepthTexture();
		ID3D11ShaderResourceView* GetShaderView();
		ID3D11ShaderResourceView* GetDepthStencilView();
		ID3D11RenderTargetView*	GetRenderTargetView();
		ID3D11DepthStencilView* GetDepthView();
		void SetTexture(ID3D11ShaderResourceView* aShaderResource);
		static HRESULT SaveToFile(ID3D11Texture2D*& texture_resource, const std::string& file_name);
		static void CopyData(ID3D11Texture2D* dest, ID3D11Texture2D* source);
#endif

		void SaveToFile(const char* aFileName, int aSize);
		void CreateDepthStencilView(float aWidth, float aHeight, int aArraySize = 1);
	private:

		D3D11_USAGE GetUsage(int flags) const;

#ifdef SNOWBLIND_DX11
		ID3D11Texture2D* m_Texture = nullptr;
		ID3D11Texture2D* m_DepthTexture = nullptr;

		ID3D11ShaderResourceView* m_ShaderResource = nullptr;
		ID3D11ShaderResourceView* m_DepthStencilShaderView = nullptr;

		ID3D11DepthStencilView* m_DepthStencil = nullptr;
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
#endif
		int	myWidth;
		int	myHeight;

		std::string myFileName;
	};
};