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

namespace Snowblind
{
	class CDirectX11;
	class CEffect;
	class CCamera;

	struct SVertexIndexWrapper;
	struct SVertexBufferWrapper;
	struct SVertexDataWrapper;
	struct SIndexBufferWrapper;

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

		//ID3D11Buffer* myVertexBuffer;
		//ID3D11Buffer* myIndexBuffer;

		CEffect	*myEffect;

		ID3D11InputLayout *myVertexLayout;

		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
		CU::GrowingArray<SVertexTypePosCol> myVertices;
		CU::GrowingArray<int> myVertexIndices;

		SVertexIndexWrapper		*myIndexData;
		SVertexDataWrapper		*myVertexData;

		SVertexBufferWrapper	*myVertexBuffer;
		SIndexBufferWrapper		*myIndexBuffer;

		Matrix44f myOrientation;
		CDirectX11* myAPI;

	};
	
	__forceinline CEffect* CModel::GetEffect()
	{
		return myEffect;
	}
}