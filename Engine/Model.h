#pragma once
#include <string>
#include "../CommonLib/Math/Matrix/Matrix44.h"
#include <DataStructures/GrowingArray.h>
#include "VertexStructs.h"

struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11Buffer;
namespace Snowblind
{
	class CDirectX11;
	class CEffect;
	class CCamera;
	class CSurface;
	struct SVertexIndexWrapper;
	struct SVertexBufferWrapper;
	struct SVertexDataWrapper;
	struct SIndexBufferWrapper;

	class CModel 
	{
		friend class FBXFactory;
	public:
		CModel();
		~CModel();

		void CreateTriangle(const std::string& anEffectPath);
		void CreateCube(const std::string& anEffectPath, float aWidth, float aHeight, float aDepth);
		void CreateCube(const std::string& anEffectPath, const CU::Vector3f& aColor);

		void CreateTexturedCube(const std::string& anEffectPath, float aWidth, float aHeight, float aDepth);
		void CreateModel();
		void Render(CU::Matrix44f& aCameraOrientation, CU::Matrix44f& aCameraProjection);
		void RenderPolygon(CU::Matrix44f& aCameraOrientation, CU::Matrix44f& aCameraProjection);

		void AddChild(CModel* aChild);
		void SetIsSkysphere();
		void SetIsLightmesh();
		void SetPosition(const CU::Vector3f& aPosition);
		CU::Matrix44f& GetOrientation();

		void SetEffect(CEffect* anEffect);
		
	private:
		void SetMatrices(CU::Matrix44f& aCameraOrientation, CU::Matrix44f& aCameraProjection);
		ID3D11InputLayout* myVertexLayout;
		ID3D11Buffer* myConstantBuffer;

		CU::GrowingArray<SVertexTypePosCol> myVertices;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
		CU::GrowingArray<CSurface*> mySurfaces;
		CU::GrowingArray<CModel*> myChildren;

		SVertexIndexWrapper* myIndexData;
		SVertexDataWrapper* myVertexData;

		SVertexBufferWrapper* myVertexBuffer;
		SIndexBufferWrapper* myIndexBuffer;

		SVertexBaseStruct* myBaseStruct;

		CU::Matrix44f myOrientation;

		CDirectX11* myAPI;
		CCamera* myCamera;
		CEffect* myEffect;

		bool myIsLightmesh = false;
		bool myIsTextured = false;
		bool myIsNULLObject = true;
		bool myIsSkysphere = false;
		void InitVertexBuffer();
		void InitIndexBuffer();
		void InitConstantBuffer();
	};
}