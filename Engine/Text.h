#pragma once

struct ID3D11ShaderResourceView;

namespace Snowblind
{
	class CFont;
	class CCamera;
	class CText
	{
	public:
		CText(const char* aFilepath, int aSize, int aBorderWidth);
		~CText();
	
		void Render(CCamera* aCamera);
		void SetText(const std::string& aString);
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
};