#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <string>
#pragma comment(lib,"XInput9_1_0.lib")

enum class eXboxButton
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
		ControllerInput(int aPlayer);
		~ControllerInput();
		XINPUT_STATE GetState();
		bool IsConnected();

		
		void Update(float aDeltaTime);
		
		void Vibrate(unsigned short aLeftVal = 0, unsigned short aRightVal = 0, float someTime = 0);


		bool ButtonDown(int aKey);
		bool ButtonUp(int aKey);
		bool ButtonTap(int aKey);

		bool ButtonDown(eXboxButton aKey);
		bool ButtonUp(eXboxButton aKey);
		bool ButtonTap(eXboxButton aKey);

		float LeftThumbstickX();
		float LeftThumbstickY();

		float RightThumbstickX();
		float RightThumbstickY();

		float LeftTrigger();
		float RightTrigger();

		const int GetControllerID() const;

	private:

		XINPUT_STATE myControllerState;
		XINPUT_STATE myPrevControllerState;
		XINPUT_KEYSTROKE myKey;
		int myControllerID;

		float myVibrationTime;
		unsigned short myLeftMotorValue;
		unsigned short myRightMotorValue;

	};
}