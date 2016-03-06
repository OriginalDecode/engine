#pragma once
#include "../CommonLib/Math/Matrix/Matrix44.h"
#include <string>
struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11Buffer;

struct SVertexTypePosCol
{
	CU::Math::Vector3<float> myPosition;
	CU::Math::Vector4<float> myColor;
}; 

struct SVertexTypePosNormUV
{
	CU::Math::Vector3<float> myPosition;
	CU::Math::Vector3<float> myNormal;
	CU::Math::Vector2<float> myUV;
};


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
	public:
		CModel(CCamera* aCamera);
		~CModel();

		void CreateTriangle(const std::string& anEffectPath);
		void CreateCube(const std::string& anEffectPath, float aWidth, float aHeight, float aDepth);
		void CreateTexturedCube(const std::string& anEffectPath, float aWidth, float aHeight, float aDepth);
		void Render();
		void RenderPolygon();
		CEffect* GetEffect();
	private:

		void InitVertexBuffer();
		void InitIndexBuffer();

		//ID3D11Buffer* myIndexBuffer;

		CEffect	*myEffect;


		ID3D11InputLayout *myVertexLayout;

		CU::GrowingArray<SVertexTypePosCol> myVertices;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
		CU::GrowingArray<CSurface*> mySurfaces;

		SVertexIndexWrapper		*myIndexData;
		SVertexDataWrapper		*myVertexData;

		SVertexBufferWrapper	*myVertexBuffer;
		SIndexBufferWrapper		*myIndexBuffer;

		Matrix44f myOrientation;
		CDirectX11* myAPI;
		bool myIsTextured;
		CCamera* myCamera;
	};
	
	__forceinline CEffect* CModel::GetEffect()
	{
		return myEffect;
	}
}