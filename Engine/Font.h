#pragma once
#include "VertexStructs.h"

struct ID3D11InputLayout;
struct D3D11_BUFFER_DESC;
struct D3D11_INPUT_ELEMENT_DESC;
struct D3D11_SUBRESOURCE_DATA;

namespace Snowblind
{
	struct SVertexBufferWrapper;
	struct SIndexBufferWrapper;
	struct SVertexDataWrapper;
	struct SVertexIndexWrapper;

	struct SFontData;
	class CEffect;
	class CFont
	{
	public:
		CFont(SFontData* aFontData);
		~CFont();

		void SetText(const std::string& aText);
		const std::string& GetText() const;
		void Render();
		CEffect* GetEffect();
	private:
		void operator=(const CFont&) = delete;

		void CreateInputLayout();
		void CreateVertexBuffer();
		void CreateIndexBuffer();
		void UpdateBuffer();
		SFontData* myData;
		CEffect* myEffect;
		ID3D11InputLayout* myVertexLayout;

		D3D11_BUFFER_DESC* myVertexBufferDesc;
		D3D11_BUFFER_DESC* myIndexBufferDesc;
		D3D11_SUBRESOURCE_DATA* myInitData;

		SVertexIndexWrapper* myIndexData;
		SVertexDataWrapper* myVertexData;

		SVertexBufferWrapper* myVertexBuffer;
		SIndexBufferWrapper* myIndexBuffer;



		std::string myText;
		CU::GrowingArray<SVertexTypePosUV> myVertices;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
		CU::GrowingArray<int> myIndices;
	};
}