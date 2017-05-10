#pragma once


class Texture;
class GBuffer
{
public:
	GBuffer() = default;

	void Initiate();
	void CleanUp();
	void Clear(float* aClearColor);

	void SetAsRenderTarget(Texture* aDepthTexture);

	Texture* GetDiffuse();
	Texture* GetNormal();
	Texture* GetEmissive();
	Texture* GetDepth();

private:
	Texture* myAlbedo = nullptr;
	Texture* myNormal = nullptr;
	Texture* myEmissive = nullptr;
	Texture* myDepth = nullptr;
	Engine* myEngine = nullptr;
#ifdef SNOWBLIND_DX11
	DirectX11* myDirectX = nullptr;
	ID3D11DeviceContext* myContext = nullptr;
#endif
};
