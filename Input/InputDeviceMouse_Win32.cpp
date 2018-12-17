#include "InputDeviceMouse_Win32.h"

#include <cassert>
#include <dinput.h>

namespace Input
{
	InputDeviceMouse_Win32::InputDeviceMouse_Win32(HWND window_handle, HINSTANCE instance_handle)
	{
		m_WindowHandle = window_handle;
		m_Type = DeviceType::MOUSE;

		HRESULT hr = DirectInput8Create(instance_handle, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_Input, nullptr);
		assert(hr == S_OK && "Failed to create dinput8");

		hr = m_Input->CreateDevice(GUID_SysMouse, &m_Device, nullptr);
		assert(hr == S_OK && "Failed to create input device!");

		hr = m_Device->SetDataFormat(&c_dfDIMouse);
		assert(hr == S_OK && "Failed to set data format!");

		hr = m_Device->SetCooperativeLevel(window_handle, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
		if (hr != S_OK)
		{
			assert(!"Failed to set cooperative level on device!");
			Release();
			return;
		}

		hr = m_Device->Acquire();
		if (hr != S_OK)
		{
			assert(!"Failed to acquire input device!");
			Release();
			return;
		}
	}

	InputDeviceMouse_Win32::~InputDeviceMouse_Win32()
	{
	}

	bool InputDeviceMouse_Win32::OnDown(uint8 key) const
	{
		return (m_State.rgbButtons[key] & 0x80) != 0 && (m_PrevState.rgbButtons[key] & 0x80) == 0;
	}

	bool InputDeviceMouse_Win32::OnRelease(uint8 key) const
	{
		return (m_State.rgbButtons[key] & 0x80) == 0 && (m_PrevState.rgbButtons[key] & 0x80) != 0;
	}

	bool InputDeviceMouse_Win32::IsDown(uint8 key) const
	{
		return (m_State.rgbButtons[key] & 0x80) != 0;
	}

	void InputDeviceMouse_Win32::Update()
	{
		memcpy_s(&m_PrevState, sizeof(m_PrevState), &m_State, sizeof(m_State));
		HRESULT hr = m_Device->GetDeviceState(sizeof(DIMOUSESTATE), (void**)&m_State);
		if (FAILED(hr))
		{
			ZeroMemory(&m_State, sizeof(m_State));
			hr = m_Device->Acquire();
			if (hr != S_OK)
			{
				// error handling
			}
		}

		POINT cursor_point;
		GetPhysicalCursorPos(&cursor_point);
		ScreenToClient(m_WindowHandle, &cursor_point);
		m_Cursor.x = cursor_point.x;
		m_Cursor.y = cursor_point.y;

	}

	void InputDeviceMouse_Win32::Release()
	{
		m_Device->Unacquire();

		m_Device->Release();
		m_Device = nullptr;

		m_Input->Release();
		m_Input = nullptr;
	}

}; // namespace Input