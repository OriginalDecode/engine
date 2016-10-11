#pragma once

#ifndef SNOWBLIND_VULKAN

namespace CommonUtilities
{
	namespace Input
	{
		class InputWrapper;
	}
	class TimeManager;
}

namespace Snowblind
{
	class CSprite;
	class CCamera;
	class CText;
	class CScene;

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
		void SetWorldScene(CScene* aScene);
		void Set2DScene(CScene* aScene);
	private:
		CU::GrowingArray<std::string> myStrings;
		CU::Input::InputWrapper* myInputWrapper;
		CU::TimeManager* myTimeManager;
		CCamera* myCamera;
		CSprite* mySprite;

		CText* myText;
		CText* myInputText;

		CU::Math::Vector2<float> myTopLeftPosition;
		CU::Math::Vector2<float> myBottomLeftPosition;

		std::string myInput;
		std::string myMarkedText;
		std::string myCopiedText;

		CScene* myWorldScene;
		CScene* my2DScene;


		bool myIsActive;
		float myDeltaTime;
		float myDownTime;
		float myEraseTime;
		void ReadInput();
	};

	__forceinline bool CConsole::GetIsActive()
	{
		return myIsActive;
	}
};
#endif