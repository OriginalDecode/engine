#pragma once
#include "engine_shared.h"
#include <DL_Debug.h>
#include "VertexStructs.h"
#include <Engine/GBuffer.h>

class Effect;
class Texture;
class Quad;

namespace graphics
{
	class RenderContext;
}

class DeferredRenderer
{
public:
	DeferredRenderer();
	~DeferredRenderer();
	void DeferredRender(const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir, const graphics::RenderContext& render_context);

	void Finalize();
	Texture* GetScene() { return m_Scene; }
	Texture* GetDepthStencil() { return m_DepthStencilTexture; }


	void SetColor(const CU::Vector4f& dir_color) { m_ConstantStruct.m_LightColor = dir_color; }

	void OnResize();

private:
	void UpdateConstantBuffer(const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir);

	Quad* m_RenderQuad = nullptr;

	Effect* m_AmbientPassShader = nullptr;
	Effect* m_ScreenPassShader = nullptr;

	Texture* m_DepthStencilTexture = nullptr;
	Texture* m_Scene = nullptr;

	IBuffer* m_ConstantBuffer; 

	struct ConstantStruct
	{
		CU::Matrix44f m_ShadowMVP;
		CU::Vector4f m_Direction;
		CU::Vector4f m_LightColor;

	} m_ConstantStruct;
};
