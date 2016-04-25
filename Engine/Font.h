#pragma once
#include "VertexStructs.h"
#include "Utilities.h"

struct ID3D11InputLayout;
struct D3D11_BUFFER_DESC;
struct D3D11_INPUT_ELEMENT_DESC;
struct D3D11_SUBRESOURCE_DATA;
struct ID3D11ShaderResourceView;

namespace CommonUtilities
{
	class TimeManager;
}



namespace Snowblind
{
	struct SInstance
	{
		CU::Math::Vector2<float> myPosition;
	};



	struct SVertexBufferWrapper;
	struct SIndexBufferWrapper;

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
		ID3D11ShaderResourceView* GetAtlas();
		const CU::Math::Vector2<float>& GetSize();
		const short GetFontPixelSize();
		float GetUpdateTime();
		float GetRenderTime();
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

		SVertexBufferWrapper* myVertexBuffer;
		SIndexBufferWrapper* myIndexBuffer;
		std::string myText;

		CU::Math::Vector2<float> mySize;
		CU::GrowingArray<SVertexTypePosColUv> myVertices;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
		CU::GrowingArray<int> myIndices;

		CU::TimeManager* myTimeManager;

		CL::SColor myDefaultColor;
		CL::SColor myColor;


		float myUpdateTime;
		float myRenderTime;

		int myUpdateTimer;
		int myRenderTimer;

	};
}