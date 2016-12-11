#pragma once
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib,"XInput9_1_0.lib")
#include <standard_datatype.hpp>

enum eXboxButton
{
	eDPAD_UP = XINPUT_GAMEPAD_DPAD_UP,
	eDPAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
	eDPAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
	eDPAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
	eSTART = XINPUT_GAMEPAD_START,
	BACK = XINPUT_GAMEPAD_BACK,
	eLT = XINPUT_GAMEPAD_LEFT_THUMB,
	eRT = XINPUT_GAMEPAD_RIGHT_THUMB,
	eLB = XINPUT_GAMEPAD_LEFT_SHOULDER,
	eRB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
	eA = XINPUT_GAMEPAD_A,
	eB = XINPUT_GAMEPAD_B,
	eX = XINPUT_GAMEPAD_X,
	eY = XINPUT_GAMEPAD_Y,
};

struct ControllerState
{
	u16 m_Buttons = 0;
	s16 m_ThumbRX = 0;
	s16 m_ThumbRY = 0;
	s16 m_ThumbLY = 0;
	s16 m_ThumbLX = 0;
	s16 m_PacketNum = 0;
	u8 m_RTrigger = 0;
	u8 m_LTrigger = 0;
	XINPUT_STATE myControllerState;
	XINPUT_STATE myPrevControllerState;
};

class ControllerInput
{
public:
	ControllerInput(int aPlayer);
	const ControllerState& GetState() const { return m_State; }
	const ControllerState& GetPrevState() const { return m_PrevState; }
	void Update();
	const int GetControllerID() const;
	//void Update(float aDeltaTime);

	void Vibrate(unsigned short aLeftVal = 0, unsigned short aRightVal = 0, float someTime = 0);

private:
	ControllerState m_State;
	ControllerState m_PrevState;


	XINPUT_STATE m_ControllerState;
	XINPUT_STATE m_PrevControllerState;
	XINPUT_KEYSTROKE myKey;
	int myControllerID;

	float myVibrationTime;
	unsigned short myLeftMotorValue;
	unsigned short myRightMotorValue;

};
