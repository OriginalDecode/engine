#pragma once

namespace CommonUtilities
{
	namespace Input
	{
		class InputWrapper;
	}
}

namespace Snowblind
{
	class CSprite;
	class CCamera;
	class CText;

	class CConsole
	{
	public:
		CConsole();
		~CConsole();

		void Initiate(CCamera* aCamera);
		void Render();
		void Update();
		void ToggleConsole();
		void PrintToConsole(const std::string& aMessage);
		bool GetIsActive();
	private:
		CU::GrowingArray<std::string> myStrings;
		CU::Input::InputWrapper* myInputWrapper;

		CCamera* myCamera;
		CSprite* mySprite;
		CText* myText;
		CText* myInputText;

		CU::Math::Vector2<float> myTopLeftPosition;
		CU::Math::Vector2<float> myBottomLeftPosition;

		std::string myInput;
		std::string myMarkedText;
		std::string myCopiedText;


		bool myIsActive;

		void ReadInput();
	};

	__forceinline bool CConsole::GetIsActive()
	{
		return myIsActive;
	}
};