#pragma once
#include "VertexStructs.h"
#include "engine_shared.h"
struct D3D11_INPUT_ELEMENT_DESC;

class Texture;
class Camera;
class Effect;

struct IndexDataWrapper;
struct VertexBufferWrapper;
struct VertexDataWrapper;
struct IndexBufferWrapper;

class SpriteModel
{
public:
	SpriteModel() = default;
	~SpriteModel();

	void Initiate(const std::string& aTexturePath, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);
	void Initiate(Texture* aShaderResource, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);
	void Initiate(const cl::CHashString<128>& path);

	void CreateVertices();


	void Render(const CU::Matrix44f& anOrientation, CU::Matrix44f& a2DCameraOrientation, const CU::Matrix44f& anOrthogonalProjectionMatrix);
	Effect* GetEffect();
	const CU::Vector2f& GetSize();
	
	void Resize(const CU::Vector2f& new_size);


	const CU::Math::Vector2<float>& GetPosition();
	void SetTexture(Texture* srv);
private:
	Texture* myTexture;

	void UpdateConstantBuffer();

	void InitiateVertexBuffer();
	void InitiateIndexBuffer();
	void InitConstantBuffer();
	void ConvertToNormalSpace();

	std::string myTexturePath;
	WindowSize myWindowSize;
	Effect* myEffect = nullptr;

	CU::Math::Vector2<float> myPosition;
	CU::Math::Vector2<float> mySize;

	IndexDataWrapper* myIndexData;
	VertexDataWrapper* myVertexData;

	VertexBufferWrapper* myVertexBuffer;
	IndexBufferWrapper* myIndexBuffer;

	ID3D11Buffer* myConstantBuffer = nullptr;
	CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;
	IInputLayout* myVertexLayout;
	CU::GrowingArray<VertexTypePosUV> myVertices;

	struct SSpriteConstantBuffer : public VertexBaseStruct
	{
		CU::Vector2f position = CU::Vector2f(0.f, 0.f);
		CU::Vector2f scale = CU::Vector2f(1.f, 1.f);
	} m_cbStruct;


	void SetMatrices(const CU::Matrix44f& anOrientation, CU::Matrix44f& a2DCameraOrientation, const CU::Matrix44f& anOrthogonalProjectionMatrix);
};
