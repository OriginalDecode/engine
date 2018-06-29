#ifndef IS_SHADER
#pragma once
class Texture;
namespace graphics
{
	typedef Texture* float4;
	class RenderContext;
	class GBuffer
	{
	public:
		GBuffer();
		~GBuffer();

		void Initiate(bool bind_textures);

		void Clear(const float* clear_color, const RenderContext& render_context);
		void SetAsRenderTarget(Texture* depth, const RenderContext& render_context);

		Texture* GetDiffuse() const { return m_Albedo; }
		Texture* GetNormal() const { return m_Normal; }
		Texture* GetEmissive() const { return m_Emissive; }
		Texture* GetDepth() const { return m_Depth; }
#ifdef _DEBUG
		Texture* GetIDTexture() const { return m_EntityIDTexture; }
		Texture* GetRoughness() const { return m_Roughness; }
		Texture* GetMetalness() const { return m_Metalness; }
#endif
	private:
#endif
#ifdef IS_SHADER
struct  GBuffer
{
#endif

		float4 m_Albedo; // rgb, metal
		float4 m_Normal; // rgb, rough
		float4 m_Depth; // r, emissive, spec, ao
		float4 m_Emissive;
	
#ifdef _DEBUG 
		float4 m_EntityIDTexture; // 
		float4 m_Roughness;
		float4 m_Metalness;
#endif
#ifdef IS_SHADER
};
#else
	};
};
#endif