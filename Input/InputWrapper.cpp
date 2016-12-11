#include "InputWrapper.h"
#include <assert.h>

bool InputWrapper::Initiate(HWND aHWND, HINSTANCE hInstance)
{
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&myDInput, nullptr);
	myDInput->CreateDevice(GUID_SysKeyboard, &myKeyboard, nullptr);
	myDInput->CreateDevice(GUID_SysMouse, &myMouse, nullptr);

	myKeyboard->SetDataFormat(&c_dfDIKeyboard);
	myMouse->SetDataFormat(&c_dfDIMouse);

	myKeyboard->SetCooperativeLevel(myHWND, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	myMouse->SetCooperativeLevel(myHWND, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	myKeyboard->Acquire();
	myMouse->Acquire();

	myHWND = aHWND;
	return true;
}

void InputWrapper::Update()
{
	memcpy_s(&myPrevKeyState, sizeof(myPrevKeyState), myKeyState, sizeof(myKeyState));
	HRESULT hr = myKeyboard->GetDeviceState(sizeof(myKeyState), (VOID**)&myKeyState);
	if (FAILED(hr))
	{
		ZeroMemory(myKeyState, sizeof(myKeyState));
		myKeyboard->Acquire();
	}

	memcpy_s(&myPrevMouseState, sizeof(myPrevMouseState), &myMouseState, sizeof(myMouseState));
	hr = myMouse->GetDeviceState(sizeof(DIMOUSESTATE), (VOID**)&myMouseState);
	if (FAILED(hr))
	{
		ZeroMemory(&myMouseState, sizeof(myMouseState));
		myMouse->Acquire();
	}

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

bool InputWrapper::IsDown(MouseInput button)
{
	return (myMouseState.rgbButtons[(s32)button] & 0x80) != 0;
}

bool InputWrapper::OnRelease(MouseInput button)
{
	return (myMouseState.rgbButtons[(s32)button] & 0x80) == 0 && (myPrevMouseState.rgbButtons[(s32)button] & 0x80) != 0;
}

bool InputWrapper::OnClick(MouseInput button)
{
	return (myMouseState.rgbButtons[(s32)button] & 0x80) != 0 && (myPrevMouseState.rgbButtons[(s32)button] & 0x80) == 0;
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