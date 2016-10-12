#pragma once
#pragma message ("Texture.h is being compiled")
#ifdef SNOWBLIND_DX11
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
#endif
namespace Snowblind
{
	class CTexture
	{
	public:
		CTexture();
		CTexture(float aWidth, float aHeight, unsigned int aBindFlag, unsigned int aFormat);
		~CTexture();

		void InitAsDepthBuffer(float aWidth, float aHeight);
		void InitStencil(float aWidth, float aHeight);
		const std::string& GetFileName();
		bool LoadTexture(const std::string& aFileName);

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
		void CopyData(ID3D11Texture2D* source);
#endif

		void SaveToFile(const char* aFileName, int aSize);
		void CreateDepthStencilView(float aWidth, float aHeight, int aArraySize = 1);
	private:

#ifdef SNOWBLIND_DX11
		ID3D11Texture2D* myTexture;
		ID3D11ShaderResourceView* myShaderResource;
		ID3D11Texture2D* myDepthTexture;
		ID3D11DepthStencilView* myDepthStencil;
		ID3D11ShaderResourceView* myDepthStencilShaderView;
		ID3D11RenderTargetView* myRenderTargetView;
#endif
		int	myWidth;
		int	myHeight;

		std::string myFileName;
	};
};