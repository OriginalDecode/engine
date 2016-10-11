#pragma once
#include "VertexStructs.h"
#ifndef SNOWBLIND_VULKAN
struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11ShaderResourceView;
#endif
namespace Snowblind
{
	class CTexture;
	class CCamera;
	class CEffect;

	struct SVertexIndexWrapper;
	struct SVertexBufferWrapper;
	struct SVertexDataWrapper;
	struct SIndexBufferWrapper;

	class CSpriteModel
	{
	public:
		CSpriteModel();
		~CSpriteModel();

		void Initiate(const std::string& aTexturePath, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);
		void Initiate(ID3D11ShaderResourceView* aShaderResource, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);

		void Render(const CU::Matrix44f& anOrientation, CU::Matrix44f& a2DCameraOrientation, const CU::Matrix44f& anOrthogonalProjectionMatrix);
		CEffect* GetEffect();
		CU::Math::Vector2<float> GetSize();
		const CU::Math::Vector2<float>& GetPosition();

	private:

		void UpdateConstantBuffer();

		void InitiateVertexBuffer();
		void InitiateIndexBuffer();
		void InitConstantBuffer();
		void ConvertToNormalSpace();

		std::string myTexturePath;
		SWindowSize myWindowSize;
		CCamera* myCamera;
		CEffect* myEffect;

		CU::Math::Vector2<float> myPosition;
		CU::Math::Vector2<float> mySize;

		SVertexIndexWrapper* myIndexData;
		SVertexDataWrapper* myVertexData;

		SVertexBufferWrapper* myVertexBuffer;
		SIndexBufferWrapper* myIndexBuffer;

#ifndef SNOWBLIND_VULKAN
		ID3D11ShaderResourceView* myTexture;
		ID3D11Buffer* myConstantBuffer = nullptr;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
		ID3D11InputLayout* myVertexLayout;
#endif
		CU::GrowingArray<SVertexTypePosUV> myVertices;

		struct SSpriteConstantBuffer : public SVertexBaseStruct
		{
			CU::Vector2f position = CU::Vector2f(0.f, 0.f);
			CU::Vector2f scale = CU::Vector2f(1.f, 1.f);
		} *myConstantStruct;


		void SetMatrices(const CU::Matrix44f& anOrientation, CU::Matrix44f& a2DCameraOrientation, const CU::Matrix44f& anOrthogonalProjectionMatrix);
	};
};