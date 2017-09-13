#pragma once
#include "engine_shared.h"
#include <DL_Debug.h>
#include "VertexStructs.h"
#include <Engine/GBuffer.h>

enum eDeferredType
{
	NONE,
	ALBEDO,
	NORMAL,
	DEPTH,
};

class Effect;
class Texture;
class Quad;

class DeferredRenderer
{
public:
	DeferredRenderer() = default;
	~DeferredRenderer();
	bool Initiate(Texture* shadow_texture);
	void SetGBufferAsTarget();
	void SetBuffers();
	void DeferredRender(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& aProjection, const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir);
	void SetRenderTarget();

	Texture* GetFinalTexture() { return m_FinishedSceneTexture; }
	Texture* GetSampleTexture() { return m_SampleTexture; }
	Texture* GetDepthStencil() { return m_DepthStencil; }

	GBuffer& GetGBuffer() { return m_GBuffer; }

	void SetColor(const CU::Vector4f& dir_color) { m_ConstantStruct.m_LightColor = dir_color; }//this is for the dir light only

	void OnResize();

private:
	void UpdateConstantBuffer(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& aProjection, const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir);
	void InitConstantBuffer();
	GBuffer m_GBuffer;

	Quad* m_RenderQuad = nullptr;

	//eh
	Effect* m_AmbientPassShader = nullptr;
	Effect* m_ScreenPassShader = nullptr;

	Texture* m_DepthStencil = nullptr;
	Texture* m_SampleTexture = nullptr; //what is even the sample texture?
	Texture* m_FinishedSceneTexture = nullptr;

	Texture* m_CubeMap = nullptr; //might be changed



	IBuffer* m_ConstantBuffer; 
	// look over the variables
	struct SConstantStruct
	{
		CU::Vector4f camPosition;
		CU::Matrix44f invertedProjection;
		CU::Matrix44f view;
		CU::Matrix44f m_ShadowMVP;
		CU::Vector4f m_Direction;
		CU::Vector4f m_LightColor;
	} m_ConstantStruct;
};
