#pragma once
#include <DL_Debug.h>
#include "VertexStructs.h"
struct ID3D11DeviceContext;
struct ID3D11InputLayout;
struct ID3D11Buffer;

namespace Snowblind
{
	enum eDeferredType
	{
		NONE,
		ALBEDO,
		NORMAL,
		DEPTH,
	};

	class CTexture;
	class CDirectX11;
	class CEffect;
	class CEngine;
	class CCamera;
	class CPointLight;
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
		void DeferredRender();

		void SetLightStates();
		void RenderLight(CPointLight* pointlight, CCamera* aCamera, CU::Matrix44f& previousOrientation);
		void SetNormalStates();
		CTexture* GetDepthStencil();
		CTexture* GetDepth();
	private:

		struct SRenderToScreenData
		{
			CEffect* myEffect = nullptr;
		} myScreenData;

		struct SAmbientPass
		{
			CEffect* myEffect = nullptr;
		} myAmbientPass;

		struct SLightPass
		{
			CEffect* myEffect = nullptr;
			ID3D11Buffer* myPixelConstantBuffer = nullptr;
			ID3D11Buffer* myVertexConstantBuffer = nullptr;
			struct SPixelConstantBuffer
			{
				CU::Matrix44f myInvertedProjection;
				CU::Matrix44f myView;
				CU::Vector4f myColor;
				CU::Vector4f myPosition;

			} myPixelConstantStruct;

			struct SVertexConstantBuffer
			{
				CU::Matrix44f myWorld;
				CU::Matrix44f myInvertedView;
				CU::Matrix44f myProjection;
				float myScale;
				CU::Vector3f filler;
			} myVertexConstantStruct;

		} myLightPass;

		void CreateLightConstantBuffers();
		void UpdateLightBuffers(CPointLight* pointlight, CCamera* aCamera, const CU::Matrix44f& previousOrientation);


		CEngine* myEngine;
		CTexture* myAlbedo;
		CTexture* myNormal;
		CTexture* myDepth;
		CTexture* myDepthStencil;


		struct SParticlePass
		{
			CEffect* myEffect;

		} myParticlePass;



		CTexture* myParticleTexture;

		CDirectX11* myDirectX;

		SVertexIndexWrapper* myIndexData;
		SVertexDataWrapper* myVertexData;

		SVertexBufferWrapper* myVertexBuffer;
		SIndexBufferWrapper* myIndexBuffer;

		CU::GrowingArray<SVertexTypePosUV> myVertices;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
		ID3D11DeviceContext* myContext;
		ID3D11InputLayout* myInputLayout;

		float myClearColor[4];



		void CreateFullscreenQuad();

		void CreateVertexBuffer();
		void CreateIndexBuffer();

	};

	__forceinline CTexture* CDeferredRenderer::GetDepth()
	{
		DL_ASSERT_EXP(myDepth != nullptr, "Deferred Depth Texture was null!");
		return myDepth;
	}

	__forceinline CTexture* CDeferredRenderer::GetDepthStencil()
	{
		DL_ASSERT_EXP(myDepthStencil != nullptr, "Deferred Depthstencil was null!");
		return myDepthStencil;
	}



};