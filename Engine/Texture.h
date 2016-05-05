#pragma once

struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;

namespace Snowblind
{
	class CTexture
	{
	public:
		CTexture();
		CTexture(float aWidth, float aHeight, unsigned int aBindFlag);

		~CTexture();

		const char*	GetFileName();
		bool LoadTexture(const char* aFileName);

		ID3D11ShaderResourceView* GetShaderView();
		ID3D11RenderTargetView*	GetRenderTargetView();
		ID3D11RenderTargetView** GetRenderTarget();
		void SetTexture(ID3D11ShaderResourceView* aShaderResource);
		void SaveToFile(const char* aFileName, int aSize);
	private:

		ID3D11ShaderResourceView* myShaderResource;
		ID3D11RenderTargetView* myRenderTargetView;


		int	myWidth;
		int	myHeight;

		const char* myFileName;
	};
};