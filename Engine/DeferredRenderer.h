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

class Camera;
class DirectX11;
class Effect;
class Engine;
class LightPass;
class PointLight;
class Texture;

struct IndexDataWrapper;
struct VertexBufferWrapper;
struct VertexDataWrapper;
struct IndexBufferWrapper;
class DeferredRenderer
{
public:
	DeferredRenderer() = default;
	~DeferredRenderer();
	bool Initiate(Texture* shadow_texture);
	void SetGBufferAsTarget(const RenderContext& render_context);
	void SetBuffers(const RenderContext& render_context);
	void DeferredRender(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& aProjection, const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir, const RenderContext& render_context);
	void SetRenderTarget(const RenderContext& render_context);


	void Finalize(const RenderContext& render_context);

	Texture* GetFinalTexture();
	Texture* GetSampleTexture();


	//void RenderPointLight(CPointLight* pointlight, CCamera* aCamera, CU::Matrix44f& previousOrientation);
	Texture* GetDepthStencil();
	GBuffer& GetGBuffer();
	void ToggleWireframe() { m_Wireframe = !m_Wireframe; }

	void SetColor(const CU::Vector4f& dir_color) { m_ConstantStruct.m_LightColor = dir_color; }

private:
	bool m_Wireframe = false;
	void UpdateConstantBuffer(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& aProjection, const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir);

	void CreateFullscreenQuad();
	void InitConstantBuffer();
	void CreateBuffer();
	void CreateIndexBuffer();

	float myClearColor[4];
	Engine* myEngine = nullptr;

	GBuffer m_GBuffer;

	DirectX11* m_API = nullptr;

	IDevContext* myContext = nullptr;

	Effect* myAmbientPassShader = nullptr;
	Effect* myScreenPassShader = nullptr;

	Texture* myDepthStencil = nullptr;
	Texture* m_SampleTexture = nullptr;
	Texture* myFinishedSceneTexture = nullptr;
	Texture* myCubeMap = nullptr;

	VertexDataWrapper* myVertexData = nullptr;
	VertexBufferWrapper* m_VertexBuffer = nullptr;

	IndexDataWrapper* myIndexData = nullptr;
	IndexBufferWrapper* m_IndexBuffer = nullptr;

	IInputLayout* myInputLayout = nullptr;

	CU::GrowingArray<VertexTypePosUV> myVertices;
	CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;

	IBuffer* myConstantBuffer;
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

__forceinline Texture* DeferredRenderer::GetDepthStencil()
{
	DL_ASSERT_EXP(myDepthStencil != nullptr, "Deferred Depthstencil was null!");
	return myDepthStencil;
}
