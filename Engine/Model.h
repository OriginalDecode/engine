#pragma once
#include "../CommonLib/Math/Matrix/Matrix44.h"
#include <string>
struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11Buffer;


namespace Snowblind
{
	class CDirectX11;
}

struct SVertexTypePosCol
{
	Vector3f myPosition;
	Vector4f myColor;
};


namespace Snowblind
{
	class CEffect;
	class CCamera;
	class CModel
	{
	public:
		CModel();
		~CModel();

		void CreateCube(const std::string& anEffectPath, float aWidth, float aHeight, float aDepth);
		void Render();
		CEffect* GetEffect();
	private:

		void InitVertexBuffer();
		void InitIndexBuffer();

		ID3D11Buffer* myVertexBuffer;
		ID3D11Buffer* myIndexBuffer;

		CEffect	*myEffect;

		ID3D11InputLayout *myVertexLayout;

		//CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
		CU::GrowingArray<SVertexTypePosCol> myVertices;
		CU::GrowingArray<int> myVertexIndices;

		Matrix44f myOrientation;
		CDirectX11* myAPI;

	};
	
	__forceinline CEffect* CModel::GetEffect()
	{
		return myEffect;
	}
}