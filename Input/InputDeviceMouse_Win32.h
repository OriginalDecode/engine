#pragma once
#include "InputDevice.h"
#include <dinput.h>
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
		float x, dx;
		float y, dy;
		float z, dz; //wheel
	};

	class InputDeviceMouse_Win32 : public IInputDevice
	{
	public:
		InputDeviceMouse_Win32(HWND window_handle, HINSTANCE instance_handle);
		~InputDeviceMouse_Win32() override;
		bool OnDown(const EAction& action) const override;
		bool OnRelease(const EAction& action) const override;
		bool IsDown(const EAction& action) const override;

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