#pragma once


class Texture;
class GBuffer
{
public:
	GBuffer() = default;

	void Initiate();
	void CleanUp();
	void Clear(float* aClearColor, const RenderContext& render_context);

	void SetAsRenderTarget(Texture* aDepthTexture, const RenderContext& render_context);

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
