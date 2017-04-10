#pragma once
#include "VertexStructs.h"
#include "snowblind_shared.h"
#ifdef SNOWBLIND_DX11
struct D3D11_INPUT_ELEMENT_DESC;
#endif

class Texture;
class Camera;
class Effect;

struct IndexDataWrapper;
struct VertexBufferWrapper;
struct VertexDataWrapper;
struct IndexBufferWrapper;

class CSpriteModel
{
public:
	CSpriteModel();
	~CSpriteModel();

	void Initiate(const std::string& aTexturePath, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);
	void Initiate(ID3D11ShaderResourceView* aShaderResource, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);

	void Render(const CU::Matrix44f& anOrientation, CU::Matrix44f& a2DCameraOrientation, const CU::Matrix44f& anOrthogonalProjectionMatrix);
	Effect* GetEffect();
	CU::Math::Vector2<float> GetSize();
	const CU::Math::Vector2<float>& GetPosition();
	void SetTexture(ID3D11ShaderResourceView* srv);
private:
	ID3D11ShaderResourceView* myTexture;

	void UpdateConstantBuffer();

	void InitiateVertexBuffer();
	void InitiateIndexBuffer();
	void InitConstantBuffer();
	void ConvertToNormalSpace();

	std::string myTexturePath;
	WindowSize myWindowSize;
	Camera* myCamera = nullptr;
	Effect* myEffect = nullptr;

	CU::Math::Vector2<float> myPosition;
	CU::Math::Vector2<float> mySize;

	IndexDataWrapper* myIndexData;
	VertexDataWrapper* myVertexData;

	VertexBufferWrapper* myVertexBuffer;
	IndexBufferWrapper* myIndexBuffer;

#ifdef SNOWBLIND_DX11
	ID3D11Buffer* myConstantBuffer = nullptr;
	CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
	IInputLayout* myVertexLayout;
#endif
	CU::GrowingArray<VertexTypePosUV> myVertices;

	struct SSpriteConstantBuffer : public VertexBaseStruct
	{
		CU::Vector2f position = CU::Vector2f(0.f, 0.f);
		CU::Vector2f scale = CU::Vector2f(1.f, 1.f);
	} *myConstantStruct;


	void SetMatrices(const CU::Matrix44f& anOrientation, CU::Matrix44f& a2DCameraOrientation, const CU::Matrix44f& anOrthogonalProjectionMatrix);
};
