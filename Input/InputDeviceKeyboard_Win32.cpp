#include "InputDeviceKeyboard_Win32.h"
#include <dinput.h>
#include <cassert>

InputDeviceKeyboard_Win32::InputDeviceKeyboard_Win32(HWND window_handle, HINSTANCE instance_handle)
{
	DirectInput8Create(instance_handle, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_Input, nullptr);
	m_Input->CreateDevice(GUID_SysKeyboard, &m_Device, nullptr);

	HRESULT hr = m_Device->Acquire();
	if (hr != S_OK)
	{
		assert(!"Failed to acquire keyboard input device!");
		m_Input->Release();
		return;
	}

	hr = m_Device->SetCooperativeLevel(window_handle, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	assert(hr == S_OK && "Failed to set cooperative level on keyboard!");

}


InputDeviceKeyboard_Win32::~InputDeviceKeyboard_Win32()
{
	m_Device->Unacquire();
	m_Input->Release();
}

bool InputDeviceKeyboard_Win32::OnDown(uint8 key) const
{
	return (m_State[key] & 0x80) != 0 && (m_PrevState[key] & 0x80) == 0;
}

bool InputDeviceKeyboard_Win32::OnRelease(uint8 key) const
{
	return (m_State[key] & 0x80) == 0 && (m_PrevState[key] & 0x80) != 0;
}

bool InputDeviceKeyboard_Win32::IsDown(uint8 key) const
{
	return (m_State[key] & 0x80) != 0;
}

void InputDeviceKeyboard_Win32::Update()
{
	memcpy_s(&m_PrevState, sizeof(m_PrevState), &m_State, sizeof(m_State));
	HRESULT hr = m_Device->GetDeviceState(sizeof(m_State), (void**)&m_State);
	if (FAILED(hr))
	{
		ZeroMemory(m_State, sizeof(m_State));
		m_Device->Acquire();
	}
}