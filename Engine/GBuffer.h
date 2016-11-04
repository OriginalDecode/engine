#pragma once
namespace Snowblind
{

	class CTexture;
	class GBuffer
	{
	public:
		GBuffer();
		~GBuffer();

		void Clear(float* aClearColor);

		void SetAsRenderTarget(CTexture* aDepthTexture);

		CTexture* myAlbedo = nullptr;
		CTexture* myNormal = nullptr;
		CTexture* myEmissive = nullptr;
		CTexture* myDepth = nullptr;


	private:
		CEngine* myEngine = nullptr;
#ifdef SNOWBLIND_DX11
		DirectX11* myDirectX = nullptr;
		ID3D11DeviceContext* myContext = nullptr;
#endif
	};
};