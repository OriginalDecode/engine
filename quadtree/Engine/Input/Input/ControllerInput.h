#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <string>
#pragma comment(lib,"XInput9_1_0.lib")

enum class XButton
{
	A = XINPUT_GAMEPAD_A,
	B = XINPUT_GAMEPAD_B,
	Y = XINPUT_GAMEPAD_Y,
	X = XINPUT_GAMEPAD_X,
	LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
	RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
	DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
	UP = XINPUT_GAMEPAD_DPAD_UP,
	RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
	LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
	LT = XINPUT_GAMEPAD_LEFT_THUMB,
	RT = XINPUT_GAMEPAD_RIGHT_THUMB,
	START = XINPUT_GAMEPAD_START,
	BACK = XINPUT_GAMEPAD_BACK
};

namespace CommonUtilities
{
	class ControllerInput
	{
	public:
		ControllerInput();
		ControllerInput(int aPlayer);
		~ControllerInput();
		XINPUT_STATE GetState();
		bool IsConnected();

		void Vibrate(int aLeftVal = 0, int aRightVal = 0);
		void VibrateOverTime(float aDeltaTime, float aTimeToVibrate, int aLeftVal, int aRightVal);

		bool ButtonDown(int aKey);
		bool ButtonUp(int aKey);
		bool ButtonTap(int aKey);

		bool ButtonDown(XButton aKey);
		bool ButtonUp(XButton aKey);
		bool ButtonTap(XButton aKey);

		short LeftThumbstickX();
		short LeftThumbstickY();

		short RightThumbstickX();
		short RightThumbstickY();

		short LeftTrigger();
		short RightTrigger();

		int GetControllerID();

	private:

		XINPUT_STATE myControllerState;
		XINPUT_STATE myPrevControllerState;
		XINPUT_KEYSTROKE myKey;
		int myControllerID;

	};
}