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
	DeferredRenderer() = default;
	~DeferredRenderer();
	void Initiate(Texture* shadow_texture);
	void DeferredRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir, const graphics::RenderContext& render_context);
	void SetRenderTarget();

	void Finalize();
	Texture* GetFinalTexture() { return m_FinishedSceneTexture; }
	Texture* GetDepthStencil() { return m_DepthStencilTexture; }


	void SetColor(const CU::Vector4f& dir_color) { m_ConstantStruct.m_LightColor = dir_color; }//this is for the dir light only

	void OnResize();

private:
	void UpdateConstantBuffer(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir);

	Quad* m_RenderQuad = nullptr;

	Effect* m_AmbientPassShader = nullptr;
	Effect* m_ScreenPassShader = nullptr;

	Texture* m_DepthStencilTexture = nullptr;
	Texture* m_FinishedSceneTexture = nullptr;




	IBuffer* m_ConstantBuffer; 

	struct ConstantStruct
	{
		CU::Vector4f m_CameraPos;
		CU::Matrix44f m_Projection;
		CU::Matrix44f m_View;
		CU::Matrix44f m_ShadowMVP;
		// are the really needed or should they be moved to their own buffer?
		CU::Vector4f m_Direction;
		CU::Vector4f m_LightColor;
	} m_ConstantStruct;
};
