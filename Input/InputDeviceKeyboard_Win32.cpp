#include "InputDeviceKeyboard_Win32.h"
#include <cassert>

#include <dinput.h>

namespace Input
{
	InputDeviceKeyboard_Win32::InputDeviceKeyboard_Win32(HWND window_handle, HINSTANCE instance_handle)
	{
		m_Type = DeviceType::KEYBOARD;
		HRESULT hr = DirectInput8Create(instance_handle, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_Input, nullptr);
		assert(hr == S_OK && "Failed to create dinput8");
		
		hr = m_Input->CreateDevice(GUID_SysKeyboard, &m_Device, nullptr);
		assert(hr == S_OK && "Failed to create input device!");

		hr = m_Device->SetDataFormat(&c_dfDIKeyboard);
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

		ZeroMemory(&m_PrevState, 256);
		ZeroMemory(&m_State, 256);

	}

	InputDeviceKeyboard_Win32::~InputDeviceKeyboard_Win32()
	{
		Release();
	}

	void InputDeviceKeyboard_Win32::Release()
	{
		m_Device->Unacquire();

		m_Device->Release();
		m_Device = nullptr;

		m_Input->Release();
		m_Input = nullptr;
	}

	bool InputDeviceKeyboard_Win32::OnDown(const EAction& action) const
	{
		auto it = m_ActionMapping.find(action);

		if (it == m_ActionMapping.end())
			return false;

		for (uint8 key : it->second)
		{
			return (m_State[key] & 0x80) != 0 && (m_PrevState[key] & 0x80) == 0;
		}
		return false;
	}

	bool InputDeviceKeyboard_Win32::OnRelease(const EAction& action) const
	{
		auto it = m_ActionMapping.find(action);

		if (it == m_ActionMapping.end())
			return false;

		for (uint8 key : it->second)
		{
			return (m_State[key] & 0x80) == 0 && (m_PrevState[key] & 0x80) != 0;
		}
		return false;
	}

	bool InputDeviceKeyboard_Win32::IsDown(const EAction& action) const
	{
		auto it = m_ActionMapping.find(action);

		if (it == m_ActionMapping.end())
			return false;
		
		for (uint8 key : it->second)
		{
			return (m_State[key] & 0x80) != 0;
		}
		return false;
	}

	void InputDeviceKeyboard_Win32::Update()
	{
		memcpy_s(&m_PrevState, sizeof(m_PrevState), &m_State, sizeof(m_State));
		HRESULT hr = m_Device->GetDeviceState(sizeof(m_State), (void**)&m_State);
		if (FAILED(hr))
		{
			ZeroMemory(m_State, sizeof(m_State));
			hr = m_Device->Acquire();
			if (hr != S_OK)
			{
			}
		}
	}
}; //namespace Input