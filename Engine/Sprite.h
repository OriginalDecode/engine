#pragma once
#include <Math/Vector/Vector.h>
#include <DataStructures/GrowingArray.h>

struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11Buffer;

struct SVertexTypePos
{
	CU::Math::Vector3<float> myPosition;
	CU::Math::Vector2<float> myUV;

};

namespace Snowblind
{
	class CTexture;
	class CCamera;
	class CEffect;
	
	struct SVertexIndexWrapper;
	struct SVertexBufferWrapper;
	struct SVertexDataWrapper;
	struct SIndexBufferWrapper;

	class CSprite
	{
	public:
		CSprite(CCamera* aCamera);
		~CSprite();
		void Initiate(const char* aTexturePath, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);
		void Render();

	private:
		void InitiateVertexBuffer();
		void InitiateIndexBuffer();

		const char* myTexturePath;
		SWindowSize myWindowSize;
		CTexture* myTexture;
		CCamera* myCamera;
		CEffect* myEffect;

		CU::Math::Vector2<float> myPosition;
		CU::Math::Vector2<float> mySize;

		SVertexIndexWrapper* myIndexData;
		SVertexDataWrapper* myVertexData;

		SVertexBufferWrapper* myVertexBuffer;
		SIndexBufferWrapper* myIndexBuffer;

		CU::GrowingArray<SVertexTypePos> myVertices;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
		ID3D11InputLayout* myVertexLayout;
	};
};