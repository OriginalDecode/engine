#pragma once
struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11ShaderResourceView;
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

		void Initiate(const char* aTexturePath, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);
		void Initiate(ID3D11ShaderResourceView* aShaderResource, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);

		void Render();
		CEffect* GetEffect();
		CU::Math::Vector2<float> GetSize();
		const CU::Math::Vector2<float>& GetPosition();

	private:
		void InitiateVertexBuffer();
		void InitiateIndexBuffer();
		void ConvertToNormalSpace();
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

		CU::GrowingArray<SVertexTypePosColUv> myVertices;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
		ID3D11InputLayout* myVertexLayout;


	};
};