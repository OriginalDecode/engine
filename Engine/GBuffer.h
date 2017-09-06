#pragma once


class Texture;
class GBuffer
{
public:
	GBuffer() = default;
	~GBuffer();
	void Initiate();
	void Clear(float* aClearColor);

	void SetAsRenderTarget(Texture* aDepthTexture);

	Texture* GetDiffuse() const;
	Texture* GetNormal() const;
	Texture* GetEmissive() const;
	Texture* GetDepth() const;

private:
	Texture* myAlbedo = nullptr;
	Texture* myNormal = nullptr;
	Texture* myEmissive = nullptr;
	Texture* myDepth = nullptr;
};
