#pragma once
#include <string>
#include "../CommonLib/Math/Matrix/Matrix44.h"
#include <DataStructures/GrowingArray.h>
#include "VertexStructs.h"

struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11Buffer;

enum eModelStates : int
{
	NULL_OBJECT,
	LIGHT_MESH,
	SKY_SPHERE,
	TEXTURED,
	_COUNT
};

namespace Snowblind
{
	class CCamera;
	class CDirectX11;
	class CEffect;
	class CEngine;
	class CSurface;

	struct SVertexIndexWrapper;
	struct SVertexBufferWrapper;
	struct SVertexDataWrapper;
	struct SIndexBufferWrapper;



	class CModel 
	{
		friend class FBXFactory;
		friend class CModelImporter;
	public:
		CModel();
		~CModel();

		void CreateTriangle(const std::string& anEffectPath);
		void CreateCube(const std::string& anEffectPath, float aWidth, float aHeight, float aDepth);
		void CreateCube(const std::string& anEffectPath, const CU::Vector3f& aColor);

		void CreateTexturedCube(const std::string& anEffectPath, float aWidth, float aHeight, float aDepth);
		CModel* CreateModel();
		void Render(CU::Matrix44f& aCameraOrientation, CU::Matrix44f& aCameraProjection);
		void RenderPolygon(CU::Matrix44f& aCameraOrientation, CU::Matrix44f& aCameraProjection);

		void AddChild(CModel* aChild);
		void SetIsSkysphere();
		void SetIsLightmesh();
		void SetPosition(const CU::Vector3f& aPosition);
		CU::Matrix44f& GetOrientation();

		void SetEffect(CEffect* anEffect);
		
	private:
		ID3D11InputLayout* myVertexLayout = nullptr;
		ID3D11Buffer* myConstantBuffer = nullptr;

		CU::GrowingArray<SVertexTypePosCol> myVertices;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
		CU::GrowingArray<CSurface*> mySurfaces;
		CU::GrowingArray<CModel*> myChildren;

		SVertexIndexWrapper* myIndexData = nullptr;
		SVertexDataWrapper* myVertexData = nullptr;

		SVertexBufferWrapper* myVertexBuffer = nullptr;
		SIndexBufferWrapper* myIndexBuffer = nullptr;

		SVertexBaseStruct* myBaseStruct = nullptr;

		CU::Matrix44f myOrientation;

		CDirectX11* myAPI = nullptr;
		CEngine* myEngine = nullptr;
		CCamera* myCamera = nullptr;
		CEffect* myEffect = nullptr;

		/* Use bitset instead */
		std::bitset<eModelStates::_COUNT> myModelStates;

		void InitVertexBuffer();
		void InitIndexBuffer();
		void InitConstantBuffer();
		void SetMatrices(CU::Matrix44f& aCameraOrientation, CU::Matrix44f& aCameraProjection);
	};
}