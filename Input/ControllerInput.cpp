#include "ControllerInput.h"
ControllerInput::ControllerInput(int aPlayer)
{
	myControllerID = aPlayer;
}

bool ControllerInput::IsConnected()
{
	memcpy_s(&myPrevControllerState, sizeof(myPrevControllerState), &myControllerState, sizeof(myControllerState));
	DWORD stateResult = XInputGetState(myControllerID, &myControllerState);
	if (stateResult != ERROR_SUCCESS)
	{
		return false;
	}


	m_PrevState = m_State;

	m_State.m_Buttons = myControllerState.Gamepad.wButtons;
	m_State.m_ThumbRX = myControllerState.Gamepad.sThumbRX;
	m_State.m_ThumbRY = myControllerState.Gamepad.sThumbRY;
	m_State.m_ThumbLY = myControllerState.Gamepad.sThumbLY;
	m_State.m_ThumbLX = myControllerState.Gamepad.sThumbLX;
	m_State.m_RTrigger = myControllerState.Gamepad.bRightTrigger;
	m_State.m_LTrigger = myControllerState.Gamepad.bLeftTrigger;

	return true;
}

const int ControllerInput::GetControllerID() const //Get the controllerID, required to controll a player (as an example)
{
	return myControllerID;
}

void ControllerInput::Update(float aDeltaTime)
{
	myVibrationTime -= aDeltaTime;
	if (myVibrationTime < 0)
	{
		XINPUT_VIBRATION vibration;
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
		vibration.wLeftMotorSpeed = 0;
		vibration.wRightMotorSpeed = 0;
		XInputSetState(myControllerID, &vibration);
	}

}

void ControllerInput::Vibrate(unsigned short aLeftVal, unsigned short aRightVal, float someTime)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = myLeftMotorValue;
	vibration.wRightMotorSpeed = myRightMotorValue;
	XInputSetState(myControllerID, &vibration);

	myVibrationTime = someTime;
	myRightMotorValue = aRightVal;
	myLeftMotorValue = aLeftVal;
}