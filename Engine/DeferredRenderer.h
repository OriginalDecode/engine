#pragma once
#include "BadValueException.h"

struct ID3DX11EffectShaderResourceVariable;
struct ID3D11ShaderResourceView;
struct ID3DX11EffectVariable;
struct ID3DX11EffectMatrixVariable;
struct ID3D11DeviceContext;
struct ID3D11InputLayout;

namespace Snowblind
{	enum eDeferredType
	{
		ALBEDO,
		NORMAL,
		DEPTH,
	};

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
		void Render();
		ID3D11ShaderResourceView* GetDeferredTexture(const eDeferredType& aDeferredType);

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
		};

		struct SLightPass
		{
			CEffect* myEffect = nullptr;
			ID3DX11EffectShaderResourceVariable* myAlbedo = nullptr;
			ID3DX11EffectShaderResourceVariable* myNormal = nullptr;
			ID3DX11EffectShaderResourceVariable* myDepth = nullptr;
			ID3DX11EffectVariable* myPointLightVariable;
		};

		CEngine* myEngine;
		CTexture* myAlbedo;
		CTexture* myNormal;
		CTexture* myDepth;
		CTexture* myDepthStencil;

		SWindowSize myWindowSize;
		SAmbientPass myAmbientPass;
		SLightPass myLightPass;
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

		void DeferredRender();

		void CreateLightData();
		void CreateAmbientData();

		void CreateFullscreenQuad();

		void CreateVertexBuffer();
		void CreateIndexBuffer();

	};
};