#pragma once
namespace Snowblind
{

	class CTexture;
	class CGBuffer
	{
	public:
		CGBuffer();
		~CGBuffer();

		void Clear(float* aClearColor);

		void SetAsRenderTarget(CTexture* aDepthTexture);

		CTexture* myAlbedo = nullptr;
		CTexture* myNormal = nullptr;
		CTexture* myEmissive = nullptr;
		CTexture* myDepth = nullptr;


	private:
		CEngine* myEngine = nullptr;
		CDirectX11* myDirectX = nullptr;
		ID3D11DeviceContext* myContext = nullptr;
	};
};