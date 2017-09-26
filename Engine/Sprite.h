#pragma once
#include "engine_shared.h"
#include <Math/Matrix/Matrix.h>



class Camera;
class Quad;
class Sprite
{
public:
	Sprite() = default;

	~Sprite();
	void Initiate(const std::string& aTexturePath, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);
	//void Initiate(ID3D11ShaderResourceView* aShaderResource, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);
	void Initiate(const cl::HashString& path);


	void Render(Camera* aCamera);
	const CU::Math::Vector2<float>& GetPosition();
	CU::Math::Vector2<float> GetSize();

	void SetPosition(const CU::Math::Vector2<float>& aPosition);
	void SetHotspot(const CU::Math::Vector2<float>& aHotspot);
	void SetSize(const CU::Math::Vector2<float>& aSize);
	void SetScale(const CU::Math::Vector2<float>& aScale);
	//void SetShaderView(ID3D11ShaderResourceView* srv);
private:

	Quad* m_Quad = nullptr;
	CU::Matrix44f myOrientation;
	CU::Vector2f myHotspot;
	CU::Vector2f myPosition;


	struct cbSprite : public VertexBaseStruct
	{
		CU::Vector2f position = CU::Vector2f(0.f, 0.f);
		CU::Vector2f scale = CU::Vector2f(1.f, 1.f);
	} m_cbStruct;
	IBuffer* m_cbSprite = nullptr;
};
