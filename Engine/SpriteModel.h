#pragma once
struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11ShaderResourceView;
struct ID3D11BlendState;

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

		void Initiate(const std::wstring& aTexturePath, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);
		void Initiate(ID3D11ShaderResourceView* aShaderResource, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);

		void Render();
		CEffect* GetEffect();
		CU::Math::Vector2<float> GetSize();
		const CU::Math::Vector2<float>& GetPosition();

	private:
		void InitiateVertexBuffer();
		void InitiateIndexBuffer();
		void InitiateBlendState();
		void ConvertToNormalSpace();
		std::wstring myTexturePath;
		SWindowSize myWindowSize;
		ID3D11ShaderResourceView* myTexture;
		CCamera* myCamera;
		CEffect* myEffect;

		CU::Math::Vector2<float> myPosition;
		CU::Math::Vector2<float> mySize;

		SVertexIndexWrapper* myIndexData;
		SVertexDataWrapper* myVertexData;

		SVertexBufferWrapper* myVertexBuffer;
		SIndexBufferWrapper* myIndexBuffer;

		ID3D11BlendState* myBlendState;

		CU::GrowingArray<SVertexTypePosUV> myVertices;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
		ID3D11InputLayout* myVertexLayout;


	};
};