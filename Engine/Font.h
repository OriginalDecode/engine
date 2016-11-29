#pragma once
#include "VertexStructs.h"
#include "Utilities.h"

#ifdef SNOWBLIND_DX11
struct D3D11_BUFFER_DESC;
struct D3D11_INPUT_ELEMENT_DESC;
struct D3D11_SUBRESOURCE_DATA;
#endif
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

	struct VertexBufferWrapper;
	struct IndexBufferWrapper;

	struct SFontData;
	class Effect;
	class Engine;
	class CFont
	{
	public:
		CFont(SFontData* aFontData);
		~CFont();

		void SetText(const std::string& aText);
		const std::string& GetText() const;
		void Render();
		ID3D11ShaderResourceView* GetAtlas();
		const CU::Math::Vector2<float>& GetSize();
		const short GetFontPixelSize();

		float GetUpdateTime();
		float GetRenderTime();

		void SetPosition(const CU::Vector2f& aPosition);
		void SetScale(const CU::Vector2f& aScale);
		void SetMatrices(const CU::Matrix44f& anOrientation, CU::Matrix44f& a2DCameraOrientation, const CU::Matrix44f& anOrthogonalProjectionMatrix);

	private:
		void operator=(const CFont&) = delete;
		void CreateInputLayout();
		void CreateVertexBuffer();
		void CreateIndexBuffer();
		void CreateConstantBuffer();
		void UpdateBuffer();
		void UpdateConstantBuffer();
		SFontData* myData = nullptr;
		Effect* myEffect[2];

#ifdef SNOWBLIND_DX11
		IInputLayout* myVertexLayout = nullptr;
		D3D11_BUFFER_DESC* myVertexBufferDesc = nullptr;
		D3D11_BUFFER_DESC* myIndexBufferDesc = nullptr;

		D3D11_SUBRESOURCE_DATA* myInitData = nullptr;
		ID3D11Buffer* myConstantBuffer = nullptr;
#endif

		VertexBufferWrapper* myVertexBuffer = nullptr;
		IndexBufferWrapper* myIndexBuffer = nullptr;
		std::string myText = "NONE";

		CU::Math::Vector2<float> mySize;
		CU::GrowingArray<SVertexTypePosColUv> myVertices;
#ifdef SNOWBLIND_DX11
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
#endif
		CU::GrowingArray<int> myIndices;

		CU::TimeManager* myTimeManager = nullptr;
		
		CL::SColor myDefaultColor = CL::SColor(0,0,0,0);
		CL::SColor myColor = CL::SColor(0,0,0,0);

		float myUpdateTime = 0.f;
		float myRenderTime = 0.f;

		int myUpdateTimer = 0.f;
		int myRenderTimer = 0.f;

		struct SFontConstantBuffer : public SVertexBaseStruct
		{
			CU::Vector2f position = CU::Vector2f(0.f, 0.f);
			CU::Vector2f scale = CU::Vector2f(1.f, 1.f);
		} *myConstantStruct;

		Engine* myEngine;

	};
}