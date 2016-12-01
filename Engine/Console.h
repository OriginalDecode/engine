#pragma once

#ifdef SNOWBLIND_DX11

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
	class Sprite;
	class Camera;
	class CText;
	class CScene;

	class CConsole
	{
	public:
		CConsole();
		~CConsole();

		void Initiate(Camera* aCamera);
		void Render();
		void Update();
		void ToggleConsole();
		void PrintToConsole(const std::string& aMessage);
		bool GetIsActive();
		void SetWorldScene(CScene* aScene);
		void Set2DScene(CScene* aScene);
	private:
		CU::GrowingArray<std::string> myStrings;
		CU::TimeManager* myTimeManager;
		Camera* myCamera;
		Sprite* mySprite;

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