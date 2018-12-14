#pragma once
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib,"XInput9_1_0.lib")
#include <standard_datatype.hpp>


enum eGamepad
{
	Gamepad_LUP			= XINPUT_GAMEPAD_DPAD_UP,
	Gamepad_LDOWN		= XINPUT_GAMEPAD_DPAD_DOWN,
	Gamepad_LLEFT		= XINPUT_GAMEPAD_DPAD_LEFT,
	Gamepad_LRIGHT		= XINPUT_GAMEPAD_DPAD_RIGHT,
	Gamepad_START		= XINPUT_GAMEPAD_START,
	Gamepad_BACK		= XINPUT_GAMEPAD_BACK,
	Gamepad_LTHUMB		= XINPUT_GAMEPAD_LEFT_THUMB,
	Gamepad_RTHUMB		= XINPUT_GAMEPAD_RIGHT_THUMB,
	Gamepad_LShoulder	= XINPUT_GAMEPAD_LEFT_SHOULDER,
	Gamepad_RShoulder	= XINPUT_GAMEPAD_RIGHT_SHOULDER,
	Gamepad_RUP			= XINPUT_GAMEPAD_A,
	Gamepad_RDOWN		= XINPUT_GAMEPAD_B,
	Gamepad_RLEFT		= XINPUT_GAMEPAD_X,
	Gamepad_RRIGHT		= XINPUT_GAMEPAD_Y,
};

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
	uint16 m_Buttons = 0;
	int16 m_ThumbRX = 0;
	int16 m_ThumbRY = 0;
	int16 m_ThumbLY = 0;
	int16 m_ThumbLX = 0;
	int16 m_PacketNum = 0;
	uint8 m_RTrigger = 0;
	uint8 m_LTrigger = 0;
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
	bool IsDown(eXboxButton aKey);
	bool OnDown(eXboxButton aKey);
	bool OnRelease(eXboxButton aKey);
	

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
