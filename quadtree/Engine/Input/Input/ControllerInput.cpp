#include "ControllerInput.h"
namespace CommonUtilities
{
	ControllerInput::ControllerInput()
	{

	}

	ControllerInput::ControllerInput(int aPlayer)
	{
		//Set the controller ID (0 -> 3) 
		myControllerID = aPlayer;
	}

	ControllerInput::~ControllerInput()
	{
	}

	XINPUT_STATE ControllerInput::GetState()
	{
		//Set the state to Zero.
		ZeroMemory(&myControllerState, sizeof(XINPUT_STATE));

		//Get the state of the controller
		XInputGetState(myControllerID, &myControllerState);
		//retrun the state
		return myControllerState;
	}

	bool ControllerInput::IsConnected()
	{
		//Copy the current controllerState to the Previous one, needed to check ButtonUp and ButtonTap.
		memcpy_s(&myPrevControllerState, sizeof(myPrevControllerState), &myControllerState, sizeof(myControllerState));

		//Gets the state and saves in the stateResult DWORD.
		DWORD stateResult = XInputGetState(myControllerID, &myControllerState);

		if (stateResult == ERROR_SUCCESS)
			return true;
		else
			return false;
	}

	int ControllerInput::GetControllerID() //Get the controllerID, required to controll a player (as an example)
	{
		return myControllerID;
	}

	void ControllerInput::Vibrate(int aLeftVal, int aRightVal)
	{
		XINPUT_VIBRATION Vibration;

		ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

		Vibration.wLeftMotorSpeed = aLeftVal;
		Vibration.wRightMotorSpeed = aRightVal;

		XInputSetState(myControllerID, &Vibration);
	}

	bool ControllerInput::ButtonDown(XButton aKey)
	{

		return (myControllerState.Gamepad.wButtons & int(aKey)) != 0;
	}

	bool ControllerInput::ButtonUp(XButton aKey)
	{
		return (((myControllerState.Gamepad.wButtons & int(aKey)) == 0) && ((myPrevControllerState.Gamepad.wButtons & int(aKey)) != 0));
	}

	bool ControllerInput::ButtonTap(XButton aKey)
	{
		return (((myControllerState.Gamepad.wButtons & int(aKey)) != 0) && ((myPrevControllerState.Gamepad.wButtons & int(aKey)) == 0));
	}

	bool ControllerInput::ButtonDown(int aKey)
	{

		return (myControllerState.Gamepad.wButtons & (aKey)) != 0;
	}

	bool ControllerInput::ButtonUp(int aKey)
	{
		return (((myControllerState.Gamepad.wButtons & (aKey)) == 0) && ((myPrevControllerState.Gamepad.wButtons & (aKey)) != 0));
	}

	bool ControllerInput::ButtonTap(int aKey)
	{
		return (((myControllerState.Gamepad.wButtons & (aKey)) != 0) && ((myPrevControllerState.Gamepad.wButtons & (aKey)) == 0));
	}

	short ControllerInput::LeftThumbstickX()
	{
		return myControllerState.Gamepad.sThumbLX;
	}
	short ControllerInput::LeftThumbstickY()
	{
		return myControllerState.Gamepad.sThumbLY;
	}

	short ControllerInput::RightThumbstickX()
	{
		return myControllerState.Gamepad.sThumbRX;
	}
	short ControllerInput::RightThumbstickY()
	{
		return myControllerState.Gamepad.sThumbRY;
	}

	short ControllerInput::LeftTrigger()
	{
		return myControllerState.Gamepad.bLeftTrigger;
	}
	short ControllerInput::RightTrigger()
	{
		return myControllerState.Gamepad.bRightTrigger;
	}
}
