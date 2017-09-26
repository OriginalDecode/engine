//#pragma once
//#include "VertexStructs.h"
//#include "engine_shared.h"
//#include <Engine/VertexWrapper.h>
//#include <Engine/IndexWrapper.h>
//
//class Texture;
//class Camera;
//class Effect;
//class SpriteModel
//{
//public:
//	SpriteModel() = default;
//	~SpriteModel();
//
//// 	void Initiate(const std::string& aTexturePath, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);
//// 	void Initiate(Texture* aShaderResource, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);
//// 	void Initiate(const cl::HashString& path);
//
//
//
//	void Render(const CU::Matrix44f& anOrientation, CU::Matrix44f& a2DCameraOrientation, const CU::Matrix44f& anOrthogonalProjectionMatrix);
//	Effect* GetEffect();
//	const CU::Vector2f& GetSize();
//	
//	void Resize(const CU::Vector2f& new_size);
//
//
//	const CU::Vector2f& GetPosition();
//	void SetTexture(Texture* srv);
//private:
//	void CreateVertices();
//
//	void InitiateVertexBuffer();
//	void InitiateIndexBuffer();
//	void InitConstantBuffer();
//	void ConvertToNormalSpace();
//	void UpdateConstantBuffer(const CU::Matrix44f& orientation, const CU::Matrix44f& camera_view, const CU::Matrix44f& camera_projection);
//
//	Texture* m_Texture = nullptr;
//	Effect* m_Effect = nullptr;
//
//	CU::Vector2f m_Position;
//	CU::Vector2f m_Size;
//
//	VertexWrapper m_VertexWrapper;
//	IndexWrapper m_IndexWrapper;
//
//
//	CU::GrowingArray<VertexTypePosUV> m_Vertices;
//
//	struct SSpriteConstantBuffer : public VertexBaseStruct
//	{
//		CU::Vector2f position = CU::Vector2f(0.f, 0.f);
//		CU::Vector2f scale = CU::Vector2f(1.f, 1.f);
//	} m_cbStruct;
//	IBuffer* m_cbSprite = nullptr;
//
//};
