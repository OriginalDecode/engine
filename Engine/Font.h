#pragma once
#include "VertexStructs.h"
#include "Utilities.h"

struct ID3D11InputLayout;
struct D3D11_BUFFER_DESC;
struct D3D11_INPUT_ELEMENT_DESC;
struct D3D11_SUBRESOURCE_DATA;
struct ID3D11ShaderResourceView;
struct ID3D11BlendState;

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
		SFontData* myData = nullptr;
		CEffect* myEffect = nullptr;

		ID3D11InputLayout* myVertexLayout = nullptr;
		D3D11_BUFFER_DESC* myVertexBufferDesc = nullptr;
		D3D11_BUFFER_DESC* myIndexBufferDesc = nullptr;

		D3D11_SUBRESOURCE_DATA* myInitData = nullptr;

		SVertexBufferWrapper* myVertexBuffer = nullptr;
		SIndexBufferWrapper* myIndexBuffer = nullptr;
		std::string myText = "NONE";

		CU::Math::Vector2<float> mySize;
		CU::GrowingArray<SVertexTypePosColUv> myVertices;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
		CU::GrowingArray<int> myIndices;

		CU::TimeManager* myTimeManager = nullptr;
		
		CL::SColor myDefaultColor = CL::SColor(0,0,0,0);
		CL::SColor myColor = CL::SColor(0,0,0,0);

		float myUpdateTime = 0.f;
		float myRenderTime = 0.f;

		int myUpdateTimer = 0.f;
		int myRenderTimer = 0.f;

	};
}