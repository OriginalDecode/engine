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
	struct Cursor
	{
		float x;
		float y;
		float z; //wheel
	};

	class InputDeviceMouse_Win32 : public IInputDevice
	{
	public:
		InputDeviceMouse_Win32(HWND window_handle, HINSTANCE instance_handle);
		~InputDeviceMouse_Win32() override;
		bool OnDown(uint8 key) const override;
		bool OnRelease(uint8 key) const override;
		bool IsDown(uint8 key) const override;

		void Update() override;

		const Cursor& GetCursor() const { return m_Cursor; }

	private:
		void Release() override;
		Cursor m_Cursor;
		DIMOUSESTATE2 m_State;
		DIMOUSESTATE2 m_PrevState;
		HWND m_WindowHandle = nullptr;
	};
}; //namespace Input