#pragma once
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib,"XInput9_1_0.lib")
#include <standard_datatype.hpp>

enum eXboxButton
{
	x_UP = XINPUT_GAMEPAD_DPAD_UP,
	x_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
	x_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
	x_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
	x_START = XINPUT_GAMEPAD_START,
	x_BACK = XINPUT_GAMEPAD_BACK,
	x_LT = XINPUT_GAMEPAD_LEFT_THUMB,
	x_RT = XINPUT_GAMEPAD_RIGHT_THUMB,
	x_LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
	x_RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
	x_A = XINPUT_GAMEPAD_A,
	x_B = XINPUT_GAMEPAD_B,
	x_X = XINPUT_GAMEPAD_X,
	x_Y = XINPUT_GAMEPAD_Y,
};

struct ControllerState
{
	u16 m_Buttons;

	s16 m_ThumbRX;
	s16 m_ThumbRY;
	s16 m_ThumbLY;
	s16 m_ThumbLX;
	u8 m_RTrigger;
	u8 m_LTrigger;
};

class ControllerInput
{
public:
	ControllerInput(int aPlayer);
	const ControllerState& GetState() const { return m_State; }
	bool IsConnected();
	void Update(float aDeltaTime);

	void Vibrate(unsigned short aLeftVal = 0, unsigned short aRightVal = 0, float someTime = 0);

	int ButtonDown();

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
	ControllerState m_State;
	ControllerState m_PrevState;


	XINPUT_STATE myControllerState;
	XINPUT_STATE myPrevControllerState;
	XINPUT_KEYSTROKE myKey;
	int myControllerID;

	float myVibrationTime;
	unsigned short myLeftMotorValue;
	unsigned short myRightMotorValue;

};
