#include "ControllerInput.h"
ControllerInput::ControllerInput(int aPlayer)
{
	myControllerID = aPlayer;

	memset(&m_State, 0, sizeof(ControllerState));
	memset(&m_PrevState, 0, sizeof(ControllerState));

}

void ControllerInput::Update()
{
	//memcpy_s(&m_PrevControllerState, sizeof(m_PrevControllerState), &m_ControllerState, sizeof(m_ControllerState));

	DWORD stateResult = XInputGetState(myControllerID, &m_ControllerState);
	if (stateResult != ERROR_SUCCESS)
		return;

	if (m_PrevState.m_PacketNum == m_ControllerState.dwPacketNumber)
		return;

	m_PrevState = m_State;
	m_State.m_PacketNum = m_ControllerState.dwPacketNumber;
	m_State.m_Buttons = m_ControllerState.Gamepad.wButtons;
	m_State.m_ThumbRX = m_ControllerState.Gamepad.sThumbRX;
	m_State.m_ThumbRY = m_ControllerState.Gamepad.sThumbRY;
	m_State.m_ThumbLY = m_ControllerState.Gamepad.sThumbLY;
	m_State.m_ThumbLX = m_ControllerState.Gamepad.sThumbLX;
	m_State.m_RTrigger = m_ControllerState.Gamepad.bRightTrigger;
	m_State.m_LTrigger = m_ControllerState.Gamepad.bLeftTrigger;
}

const int ControllerInput::GetControllerID() const //Get the controllerID, required to controll a player (as an example)
{
	return myControllerID;
}
//
//void ControllerInput::Update(float aDeltaTime)
//{
//	myVibrationTime -= aDeltaTime;
//	if (myVibrationTime < 0)
//	{
//		XINPUT_VIBRATION vibration;
//		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
//		vibration.wLeftMotorSpeed = 0;
//		vibration.wRightMotorSpeed = 0;
//		XInputSetState(myControllerID, &vibration);
//	}
//
//}

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

bool ControllerInput::IsDown(eXboxButton aKey)
{
	return (m_State.m_Buttons & static_cast<int>(aKey)) != 0;
}

bool ControllerInput::OnDown(eXboxButton aKey)
{
	return (((m_State.m_Buttons & static_cast<int>(aKey)) != 0) && ((m_PrevState.m_Buttons & static_cast<int>(aKey)) == 0));
}

bool ControllerInput::OnRelease(eXboxButton aKey)
{
	return (((m_State.m_Buttons & static_cast<int>(aKey)) == 0) && ((m_PrevState.m_Buttons & static_cast<int>(aKey)) != 0));
}
