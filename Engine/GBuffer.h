#pragma once

class Texture;
namespace graphics
{
	class RenderContext;
	class GBuffer
	{
	public:
		GBuffer();
		~GBuffer();

		void Clear(const float* clear_color, const RenderContext& render_context);
		void SetAsRenderTarget(Texture* depth, const RenderContext& render_context);

		Texture* GetDiffuse() const { return m_Albedo; }
		Texture* GetNormal() const { return m_Normal; }
		Texture* GetEmissive() const { return m_Emissive; }
		Texture* GetDepth() const { return m_Depth; }

#ifdef _DEBUG
		Texture* GetIDTexture() const { return m_EntityIDTexture; }
		Texture* GetHoverTexture() const { return m_HoverTexture; }
#endif
	private:
		Texture* m_Albedo = nullptr;
		Texture* m_Normal = nullptr;
		Texture* m_Emissive = nullptr;
		Texture* m_Depth = nullptr;
#ifdef _DEBUG
		Texture* m_EntityIDTexture = nullptr;
		Texture* m_HoverTexture = nullptr;
#endif
	};
};