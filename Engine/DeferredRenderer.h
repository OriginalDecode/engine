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
	class CGBuffer;
	class CLightPass;
	class CPointLight;
	class CTexture;

	struct SVertexIndexWrapper;
	struct SVertexBufferWrapper;
	struct SVertexDataWrapper;
	struct SIndexBufferWrapper;

	class CDeferredRenderer
	{
	public:
		CDeferredRenderer();
		~CDeferredRenderer();
		void SetTargets();
		void SetBuffers();
		void DeferredRender(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& aProjection);


		void Finalize();

		//void RenderPointLight(CPointLight* pointlight, CCamera* aCamera, CU::Matrix44f& previousOrientation);
		CTexture* GetDepthStencil();
		CGBuffer* GetGBuffer();
	private:
		void UpdateConstantBuffer(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& aProjection);

		void CreateFullscreenQuad();
		void InitConstantBuffer();
		void CreateVertexBuffer();
		void CreateIndexBuffer();

		float myClearColor[4];
		CEngine* myEngine = nullptr;

		CGBuffer* myGBuffer = nullptr;
#ifdef SNOWBLIND_DX11
		DirectX11* myDirectX = nullptr;
		ID3D11DeviceContext* myContext = nullptr;
		
		CEffect* myAmbientPassShader = nullptr;
		CEffect* myScreenPassShader = nullptr;

		CTexture* myDepthStencil = nullptr;
		CTexture* myFinishedSceneTexture = nullptr;
		CTexture* myCubeMap = nullptr;

		SVertexIndexWrapper* myIndexData = nullptr;
		SVertexDataWrapper* myVertexData = nullptr;
		SVertexBufferWrapper* myVertexBuffer = nullptr;
		SIndexBufferWrapper* myIndexBuffer = nullptr;

		ID3D11InputLayout* myInputLayout = nullptr;

		CU::GrowingArray<SVertexTypePosUV> myVertices;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;

		ID3D11Buffer* myConstantBuffer;
		struct SConstantStruct
		{
			CU::Vector4f camPosition;
			CU::Matrix44f invertedProjection;
			CU::Matrix44f view;
		} *myConstantStruct;
#endif
	};

#ifdef SNOWBLIND_DX11
	__forceinline CTexture* CDeferredRenderer::GetDepthStencil()
	{
		DL_ASSERT_EXP(myDepthStencil != nullptr, "Deferred Depthstencil was null!");
		return myDepthStencil;
	}
#endif
};