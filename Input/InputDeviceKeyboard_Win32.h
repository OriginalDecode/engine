#pragma once
#include "InputDevice.h"



#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif


namespace Input
{
	class InputDeviceKeyboard_Win32 final : public IInputDevice
	{
	public:
		InputDeviceKeyboard_Win32(HWND window_handle, HINSTANCE instance_handle);
		~InputDeviceKeyboard_Win32() override;

		bool OnDown(const EAction& action) const override;
		bool OnRelease(const EAction& action) const override;
		bool IsDown(const EAction& action) const override;

		void Update() override;

	private:
		void Release() override;
		uint8 m_State[256];
		uint8 m_PrevState[256];

	};
}; //namespace Input