#pragma once
struct ID3D11ShaderResourceView;

class CFont;
class Camera;

namespace graphics
{
	class RenderContext;
}


class CText
{
public:
	CText(const int8* filepath, uint16 aSize, uint16 aBorderWidth);
	~CText();

	void Render(const graphics::RenderContext& rc);
	void SetText(std::string aString);
	void SetPosition(const CU::Math::Vector2<float>& aPosition);
	void SetScale(const CU::Math::Vector2<float>& aScale);
	const CU::Math::Vector2<float>& GetScale();
	ID3D11ShaderResourceView* GetAtlas();
	float GetRenderTime();
	float GetUpdateTime();

private:
	CFont* myFont;
	CU::Math::Matrix44<float> myOrientation;
	CU::Math::Vector2<float> myPosition;
	CU::Math::Vector2<float> myScale;

};
