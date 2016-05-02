#pragma once
#include <string>
#include "../CommonLib/Math/Matrix/Matrix44.h"
#include <DataStructures/GrowingArray.h>
#include "VertexStructs.h"

struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;
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
		void Render();
		void RenderPolygon();
		CEffect* GetEffect();

		void AddChild(CModel* aChild);
		void SetEffect(CEffect* anEffect);

	private:

		void InitVertexBuffer();
		void InitIndexBuffer();

		CEffect	*myEffect;


		ID3D11InputLayout *myVertexLayout;

		CU::GrowingArray<SVertexTypePosCol> myVertices;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
		CU::GrowingArray<CSurface*> mySurfaces;
		CU::GrowingArray<CModel*> myChildren;

		SVertexIndexWrapper		*myIndexData;
		SVertexDataWrapper		*myVertexData;

		SVertexBufferWrapper	*myVertexBuffer;
		SIndexBufferWrapper		*myIndexBuffer;

		CU::Matrix44f myOrientation;

		CDirectX11* myAPI;
		CCamera* myCamera;

		bool myIsTextured;
		bool myIsNULLObject;

	};
	
	__forceinline CEffect* CModel::GetEffect()
	{
		return myEffect;
	}
}