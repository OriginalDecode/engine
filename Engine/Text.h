#pragma once

struct ID3D11ShaderResourceView;

namespace Snowblind
{
	class CFont;
	class CCamera;
	class CText
	{
	public:
		CText(const char* aFilepath, int aSize, CCamera* aCamera);
		~CText();
	
		void Render(); 
		void SetText(const std::string& aString);
		void SetPosition(const CU::Math::Vector2<float>& aPosition);
		void SetScale(const CU::Math::Vector2<float>& aScale);
		const CU::Math::Vector2<float>& GetScale();
		ID3D11ShaderResourceView* GetAtlas();
		void operator>>(const std::string& aString);
		float GetRenderTime();
		float GetUpdateTime();
	private:

		CCamera* myCamera;
		CFont* myFont;
		CU::Math::Matrix44<float> myOrientation;
		CU::Math::Vector2<float> myPosition;
		CU::Math::Vector2<float> myScale;



	};
};