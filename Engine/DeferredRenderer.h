#pragma once
#include <DL_Debug.h>
#include "VertexStructs.h"
#ifdef SNOWBLIND_DX11
struct ID3D11DeviceContext;
struct ID3D11InputLayout;
struct ID3D11Buffer;
#endif
namespace Snowblind
{
	enum eDeferredType
	{
		NONE,
		ALBEDO,
		NORMAL,
		DEPTH,
	};

	class CCamera;
	class DirectX11;
	class CEffect;
	class CEngine;
	class GBuffer;
	class LightPass;
	class CPointLight;
	class Texture;

	struct SVertexIndexWrapper;
	struct SVertexBufferWrapper;
	struct SVertexDataWrapper;
	struct SIndexBufferWrapper;

	class CDeferredRenderer
	{
	public:
		CDeferredRenderer() = default;
		bool Initiate(Texture* shadow_texture);
		bool CleanUp();
		void SetTargets();
		void SetBuffers();
		void DeferredRender(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& aProjection, const CU::Matrix44f& shadow_matrix);


		void Finalize();

		//void RenderPointLight(CPointLight* pointlight, CCamera* aCamera, CU::Matrix44f& previousOrientation);
		Texture* GetDepthStencil();
		GBuffer* GetGBuffer();
		void ToggleWireframe() { m_Wireframe = !m_Wireframe; }
	private:
		bool m_Wireframe = false;
		void UpdateConstantBuffer(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& aProjection, const CU::Matrix44f& shadow_matrix);

		void CreateFullscreenQuad();
		void InitConstantBuffer();
		void CreateVertexBuffer();
		void CreateIndexBuffer();

		float myClearColor[4];
		CEngine* myEngine = nullptr;

		GBuffer* myGBuffer = nullptr;
#ifdef SNOWBLIND_DX11
		DirectX11* myDirectX = nullptr;
		ID3D11DeviceContext* myContext = nullptr;
		
		CEffect* myAmbientPassShader = nullptr;
		CEffect* myScreenPassShader = nullptr;

		Texture* myDepthStencil = nullptr;
		Texture* myFinishedSceneTexture = nullptr;
		Texture* myCubeMap = nullptr;

		SVertexDataWrapper* myVertexData = nullptr;
		SVertexBufferWrapper* m_VertexBuffer = nullptr;

		SVertexIndexWrapper* myIndexData = nullptr;
		SIndexBufferWrapper* m_IndexBuffer = nullptr;

		ID3D11InputLayout* myInputLayout = nullptr;

		CU::GrowingArray<SVertexTypePosUV> myVertices;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;

		ID3D11Buffer* myConstantBuffer;
		struct SConstantStruct
		{
			CU::Vector4f camPosition;
			CU::Matrix44f invertedProjection;
			CU::Matrix44f view;
			CU::Matrix44f m_ShadowMVP;
		} *myConstantStruct;
#endif
	};

#ifdef SNOWBLIND_DX11
	__forceinline Texture* CDeferredRenderer::GetDepthStencil()
	{
		DL_ASSERT_EXP(myDepthStencil != nullptr, "Deferred Depthstencil was null!");
		return myDepthStencil;
	}
#endif
};