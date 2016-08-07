#pragma once
#include "VertexStructs.h"
#include <Math/Matrix/Matrix44.h>
#include <DataStructures/GrowingArray.h>

struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11Buffer;
struct ID3D11DeviceContext;

namespace Snowblind
{

	class CDirectX11;
	class CEffect;
	class CEngine;
	class CSurface;

	struct SVertexIndexWrapper;
	struct SVertexBufferWrapper;
	struct SVertexDataWrapper;
	struct SIndexBufferWrapper;

	class CBaseModel
	{
	public:
		CBaseModel();
		virtual ~CBaseModel();

		virtual void Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection);
		void SetEffect(CEffect* anEffect);
	protected:
		void InitVertexBuffer();
		void InitIndexBuffer();
		void InitConstantBuffer();
		virtual void SetMatrices(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection) = 0;

		ID3D11DeviceContext* myContext = nullptr;
		CEffect* myEffect = nullptr;
		CDirectX11* myAPI = nullptr;
		CEngine* myEngine = nullptr;
		ID3D11InputLayout* myVertexLayout = nullptr;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;



		SVertexIndexWrapper* myIndexData = nullptr;
		SVertexDataWrapper* myVertexData = nullptr;

		SVertexBufferWrapper* myVertexBuffer = nullptr;
		SIndexBufferWrapper* myIndexBuffer = nullptr;

		ID3D11Buffer* myConstantBuffer = nullptr;
		SVertexBaseStruct* myConstantStruct = nullptr;

		bool myIsNULLObject = true;
		bool myIsLightMesh = false;
	};
};