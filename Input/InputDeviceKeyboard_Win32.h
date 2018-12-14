#pragma once
#include "InputDevice.h"

#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif

class InputDeviceKeyboard_Win32 final :	public IInputDevice
{
public:
	InputDeviceKeyboard_Win32(HWND window_handle, HINSTANCE instance_handle);
	~InputDeviceKeyboard_Win32() override;

	bool OnDown(uint8 key) const override;
	bool OnRelease(uint8 key) const override;
	bool IsDown(uint8 key) const override;

	void Update() override;
};

