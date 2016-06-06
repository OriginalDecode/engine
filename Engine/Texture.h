#pragma once

struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;

namespace Snowblind
{
	class CTexture
	{
	public:
		CTexture();
		CTexture(float aWidth, float aHeight, unsigned int aBindFlag, unsigned int aFormat);
		~CTexture();

		void InitAsDepthBuffer(float aWidth, float aHeight);

		const std::string& GetFileName();
		bool LoadTexture(const std::string& aFileName);

		/*
			Set the prefix of the texture object. e.g UI - Albedo 
			then it will appear as one of these
			UI - Albedo : ShaderResource
			UI - Albedo : DepthStencil
			UI - Albedo : RenderTargetView

		*/
		void SetDebugName(const std::string& debugName);
		void CreateTexture(const std::string& aTexturePath);
		ID3D11Texture2D* GetDepthTexture();
		ID3D11ShaderResourceView* GetShaderView();
		ID3D11ShaderResourceView* GetDepthStencilView();
		ID3D11RenderTargetView*	GetRenderTargetView();
		ID3D11DepthStencilView* GetDepthView();
		void SetTexture(ID3D11ShaderResourceView* aShaderResource);

		void SaveToFile(const char* aFileName, int aSize);

		void CreateDepthStencilView(float aWidth, float aHeight, int aArraySize = 1);
		void CopyData(ID3D11Texture2D* source);
	private:

		ID3D11Texture2D* myTexture;
		ID3D11Texture2D* myDepthTexture;

		ID3D11ShaderResourceView* myShaderResource;
		ID3D11DepthStencilView* myDepthStencil;
		ID3D11ShaderResourceView* myDepthStencilShaderView;
		ID3D11RenderTargetView* myRenderTargetView;

		int	myWidth;
		int	myHeight;

		std::string myFileName;
	};
};