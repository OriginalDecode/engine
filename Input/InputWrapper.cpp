#include "InputWrapper.h"
#include <cassert>
#include "InputDeviceKeyboard_Win32.h"

bool InputWrapper::Initiate(HWND aHWND, HINSTANCE hInstance)
{
	//DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&myDInput, nullptr);
	//myDInput->CreateDevice(GUID_SysMouse, &myMouse, nullptr);
	//myMouse->SetDataFormat(&c_dfDIMouse);

	//m_Devices.Add(new Input::InputDeviceKeyboard_Win32(aHWND, hInstance));

	//myHWND = aHWND;

	//myMouse->SetCooperativeLevel(myHWND, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);


	//myDInput->CreateDevice(GUID_Joystick, &m_PS4, nullptr);
	//m_PS4->SetDataFormat(&c_dfDIJoystick);
	//m_PS4->SetCooperativeLevel(myHWND, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	//m_PS4->Acquire();


	//myMouse->Acquire();

	return true;
}

bool InputWrapper::CleanUp()
{
	//myMouse->Unacquire();
	//m_PS4->Unacquire();
	return true;
}

void InputWrapper::Update()
{
	for (Input::IInputDevice* device : m_Devices)
	{
		device->Update();
	}

	//memcpy_s(&myPrevMouseState, sizeof(myPrevMouseState), &myMouseState, sizeof(myMouseState));
	//HRESULT hr = myMouse->GetDeviceState(sizeof(DIMOUSESTATE), (void**)&myMouseState);
	//if (FAILED(hr))
	//{
	//	ZeroMemory(&myMouseState, sizeof(myMouseState));
	//	myMouse->Acquire();
	//}

	//GetPhysicalCursorPos(&myCursorPos);
	//ScreenToClient(myHWND, &myCursorPos);
	//m_CurorPos.x = float(myCursorPos.x);
	//m_CurorPos.y = float(myCursorPos.y);

}

float InputWrapper::MouseDirectX()
{
	return static_cast<float>(myMouseState.lX);
}

float InputWrapper::MouseDirectY()
{
	return static_cast<float>(myMouseState.lY);
}

bool InputWrapper::IsDown(MouseInput button)
{
	return (myMouseState.rgbButtons[(int32)button] & 0x80) != 0;
}

bool InputWrapper::OnRelease(MouseInput button)
{
	return (myMouseState.rgbButtons[(int32)button] & 0x80) == 0 && (myPrevMouseState.rgbButtons[(int32)button] & 0x80) != 0;
}

bool InputWrapper::OnClick(MouseInput button)
{
	return (myMouseState.rgbButtons[(int32)button] & 0x80) != 0 && (myPrevMouseState.rgbButtons[(int32)button] & 0x80) == 0;
}

bool InputWrapper::PS4IsDown(UCHAR button)
{
	return (m_ButtonState[UCHAR(button)] & 0x80) != 0;
}

bool InputWrapper::PS4OnDown(UCHAR button)
{
	return (m_ButtonState[UCHAR(button)] & 0x80) != 0 && (m_PrevButtonState[UCHAR(button)] & 0x80) == 0;
}

bool InputWrapper::PS4OnRelease(UCHAR button)
{
	return (m_ButtonState[UCHAR(button)] & 0x80) == 0 && (m_PrevButtonState[UCHAR(button)] & 0x80) != 0;
}
