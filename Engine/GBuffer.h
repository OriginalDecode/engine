#pragma once
namespace Snowblind
{

	class Texture;
	class GBuffer
	{
	public:
		GBuffer();
		~GBuffer();

		void Clear(float* aClearColor);

		void SetAsRenderTarget(Texture* aDepthTexture);

		Texture* myAlbedo = nullptr;
		Texture* myNormal = nullptr;
		Texture* myEmissive = nullptr;
		Texture* myDepth = nullptr;


	private:
		CEngine* myEngine = nullptr;
#ifdef SNOWBLIND_DX11
		DirectX11* myDirectX = nullptr;
		ID3D11DeviceContext* myContext = nullptr;
#endif
	};
};