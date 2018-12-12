#include "InputWrapper.h"
#include <assert.h>

bool InputWrapper::Initiate(HWND aHWND, HINSTANCE hInstance)
{
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&myDInput, nullptr);
	myDInput->CreateDevice(GUID_SysKeyboard, &myKeyboard, nullptr);
	myDInput->CreateDevice(GUID_SysMouse, &myMouse, nullptr);

	myKeyboard->SetDataFormat(&c_dfDIKeyboard);
	myMouse->SetDataFormat(&c_dfDIMouse);

	myHWND = aHWND;

	myKeyboard->SetCooperativeLevel(myHWND, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	myMouse->SetCooperativeLevel(myHWND, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);


	//myDInput->CreateDevice(GUID_Joystick, &m_PS4, nullptr);
	//m_PS4->SetDataFormat(&c_dfDIJoystick);
	//m_PS4->SetCooperativeLevel(myHWND, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	//m_PS4->Acquire();


	myKeyboard->Acquire();
	myMouse->Acquire();

	return true;
}

bool InputWrapper::CleanUp()
{
	myKeyboard->Unacquire();
	myMouse->Unacquire();
	m_PS4->Unacquire();
	return true;
}

void InputWrapper::Update()
{
	memcpy_s(&myPrevKeyState, sizeof(myPrevKeyState), myKeyState, sizeof(myKeyState));
	HRESULT hr = myKeyboard->GetDeviceState(sizeof(myKeyState), (void**)&myKeyState);
	if (FAILED(hr))
	{
		ZeroMemory(myKeyState, sizeof(myKeyState));
		myKeyboard->Acquire();
	}

	memcpy_s(&myPrevMouseState, sizeof(myPrevMouseState), &myMouseState, sizeof(myMouseState));
	hr = myMouse->GetDeviceState(sizeof(DIMOUSESTATE), (void**)&myMouseState);
	if (FAILED(hr))
	{
		ZeroMemory(&myMouseState, sizeof(myMouseState));
		myMouse->Acquire();
	}

	//memcpy_s(&m_PrevButtonState, sizeof(m_PrevButtonState), &m_ButtonState, sizeof(m_ButtonState));
	//hr = m_PS4->GetDeviceState(sizeof(DIJOYSTATE), (VOID**)&m_ButtonState);
	//if (FAILED(hr))
	//{
	//	ZeroMemory(&m_ButtonState, sizeof(m_ButtonState));
	//	m_PS4->Acquire();
	//}

	//for (int i = 0; i < 256; i++)
	//{
	//	if ((m_ButtonState[UCHAR(i)] & 0x80) != 0 && (m_PrevButtonState[UCHAR(i)] & 0x80) == 0)
	//	{
	//		int a;
	//		a = 5;
	//	}
	//}

	//Playstation 4 controller stuff
//	 	X = 49
// 		[] = 48
// 		Tri = 51
// 		Circle = 50
// 		Options = 57
// 		Share = 56
// 		RB = 53
// 		LB = 52
// 		RT = 55
// 		LT = 54
// 		PS button = 60


	GetPhysicalCursorPos(&myCursorPos);
	ScreenToClient(myHWND, &myCursorPos);
	m_CurorPos.x = float(myCursorPos.x);
	m_CurorPos.y = float(myCursorPos.y);

	//GetCursorPos(&myCursorPos);
	//ScreenToClient(myHWND, &myCursorPos);
}

float InputWrapper::MouseDirectX()
{
	return static_cast<float>(myMouseState.lX);
}

float InputWrapper::MouseDirectY()
{
	return static_cast<float>(myMouseState.lY);
}

bool InputWrapper::IsDown(KButton aKey)
{
	return (myKeyState[UCHAR(aKey)] & 0x80) != 0;
}

bool InputWrapper::OnDown(KButton aKey)
{
	return (myKeyState[UCHAR(aKey)] & 0x80) != 0 && (myPrevKeyState[UCHAR(aKey)] & 0x80) == 0;
}


bool InputWrapper::OnRelease(KButton aKey)
{
	return (myKeyState[UCHAR(aKey)] & 0x80) == 0 && (myPrevKeyState[UCHAR(aKey)] & 0x80) != 0;
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

bool InputWrapper::IsDown(UCHAR aKey)
{
	return (myKeyState[aKey] & 0x80) != 0;
}

bool InputWrapper::OnDown(UCHAR aKey)
{
	return (myKeyState[aKey] & 0x80) != 0 && (myPrevKeyState[aKey] & 0x80) == 0;
}

bool InputWrapper::OnRelease(UCHAR aKey)
{
	return (myKeyState[aKey] & 0x80) == 0 && (myPrevKeyState[aKey] & 0x80) != 0;
}