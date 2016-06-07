#pragma once

struct ID3DX11EffectShaderResourceVariable;
struct ID3D11ShaderResourceView;
struct ID3DX11EffectVariable;
struct ID3DX11EffectMatrixVariable;
struct ID3D11DeviceContext;
struct ID3D11InputLayout;

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
		void SetLightState(CCamera* aCamera);
		void SetNormalState();
		void RenderLight(CPointLight* pointlight, CCamera* aCamera, CU::Matrix44f& previousOrientation);

		CTexture* GetDepthStencil();
		CTexture* GetDepth();
	private:
		void Render(CEffect* anEffect);

		struct SRenderToScreenData
		{
			CEffect* myEffect = nullptr;
			ID3DX11EffectShaderResourceVariable* mySource = nullptr;
		};
		struct SAmbientPass
		{
			CEffect* myEffect = nullptr;
			ID3DX11EffectShaderResourceVariable* myAlbedo = nullptr;
			ID3DX11EffectShaderResourceVariable* myNormal = nullptr;
			ID3DX11EffectShaderResourceVariable* myDepth = nullptr;
		} myAmbientPass;

		struct SLightPass
		{
			CEffect* myEffect = nullptr;
			ID3DX11EffectShaderResourceVariable* myAlbedo = nullptr;
			ID3DX11EffectShaderResourceVariable* myNormal = nullptr;
			ID3DX11EffectShaderResourceVariable* myDepth = nullptr;
			ID3DX11EffectVariable* myPointLightVariable;
			ID3DX11EffectMatrixVariable* myInvertedProjection;
			ID3DX11EffectMatrixVariable* myNotInvertedView;
		} myLightPass;

		CEngine* myEngine;
		CTexture* myAlbedo;
		CTexture* myNormal;
		CTexture* myDepth;
		CTexture* myDepthStencil;


		struct SParticlePass
		{
			CEffect* myEffect;
			ID3DX11EffectShaderResourceVariable* myDiffuse = nullptr;
		} myParticlePass;

		CTexture* myParticleTexture;

		CDirectX11* myDirectX;

		SWindowSize myWindowSize;
		SRenderToScreenData myScreenData;

		SVertexIndexWrapper* myIndexData;
		SVertexDataWrapper* myVertexData;

		SVertexBufferWrapper* myVertexBuffer;
		SIndexBufferWrapper* myIndexBuffer;

		CU::GrowingArray<SVertexTypePosUV> myVertices;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
		ID3D11DeviceContext* myContext;
		ID3D11InputLayout* myInputLayout;

		float myClearColor[4];


		void CreateLightData();
		void CreateAmbientData();

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