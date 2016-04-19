#pragma once


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
	private:

		CCamera* myCamera;
		CFont* myFont;
		CU::Math::Matrix44<float> myOrientation;
		CU::Math::Vector2<float> myPosition;
	};
};