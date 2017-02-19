#pragma once
#include "snowblind_shared.h"
#include <DL_Debug.h>
#include "VertexStructs.h"

namespace Hex
{
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
	class GBuffer;
	class LightPass;
	class CPointLight;
	class Texture;

	struct VertexIndexWrapper;
	struct VertexBufferWrapper;
	struct VertexDataWrapper;
	struct IndexBufferWrapper;

	class DeferredRenderer
	{
	public:
		DeferredRenderer() = default;
		bool Initiate(Texture* shadow_texture);
		bool CleanUp();
		void SetTargets();
		void SetBuffers();
		void DeferredRender(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& aProjection, const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir);


		void Finalize(Texture* light_texture);

		//void RenderPointLight(CPointLight* pointlight, CCamera* aCamera, CU::Matrix44f& previousOrientation);
		Texture* GetDepthStencil();
		GBuffer* GetGBuffer();
		void ToggleWireframe() { m_Wireframe = !m_Wireframe; }
	private:
		bool m_Wireframe = false;
		void UpdateConstantBuffer(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& aProjection, const CU::Matrix44f& shadow_mvp, const CU::Vector4f light_dir);

		void CreateFullscreenQuad();
		void InitConstantBuffer();
		void CreateVertexBuffer();
		void CreateIndexBuffer();

		float myClearColor[4];
		Engine* myEngine = nullptr;

		GBuffer* myGBuffer = nullptr;

#ifdef SNOWBLIND_DX11
		DirectX11* m_API = nullptr;
#else
		Vulkan* m_API = nullptr;
#endif

		IDevContext* myContext = nullptr;
		
		Effect* myAmbientPassShader = nullptr;
		Effect* myScreenPassShader = nullptr;

		Texture* myDepthStencil = nullptr;
		Texture* myFinishedSceneTexture = nullptr;
		Texture* myCubeMap = nullptr;

		VertexDataWrapper* myVertexData = nullptr;
		VertexBufferWrapper* m_VertexBuffer = nullptr;

		VertexIndexWrapper* myIndexData = nullptr;
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
		} m_ConstantStruct;
	};

	__forceinline Texture* DeferredRenderer::GetDepthStencil()
	{
		DL_ASSERT_EXP(myDepthStencil != nullptr, "Deferred Depthstencil was null!");
		return myDepthStencil;
	}
};