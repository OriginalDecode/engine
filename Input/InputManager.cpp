#include "InputManager.h"
#include "InputDevice.h"

#include <dinput.h>

#include "InputDeviceKeyboard_Win32.h"
#include "InputDeviceMouse_Win32.h"

namespace Input
{
	InputManager::InputManager(HWND window_handle, HINSTANCE window_instance)
	{

		m_Devices.emplace_back(new InputDeviceKeyboard_Win32(window_handle, window_instance));
		m_Devices.emplace_back(new InputDeviceMouse_Win32(window_handle, window_instance));
		m_Mouse = m_Devices.back();


		AddMapping(A_MOVE_FORWARD, DIK_W, DeviceType::KEYBOARD);
		AddMapping(A_MOVE_BACK, DIK_S, DeviceType::KEYBOARD);
		AddMapping(A_MOVE_LEFT, DIK_A, DeviceType::KEYBOARD);
		AddMapping(A_MOVE_RIGHT, DIK_D, DeviceType::KEYBOARD);
		AddMapping(A_MOVE_UP, DIK_SPACE, DeviceType::KEYBOARD);
		AddMapping(A_MOVE_DOWN, DIK_X, DeviceType::KEYBOARD);

		AddMapping(A_ROTATE_CAMERA, 1, DeviceType::MOUSE);


	}


	InputManager::~InputManager()
	{
		for (IInputDevice* device : m_Devices)
		{
			delete device;
			device = nullptr;
		}
	}

	bool InputManager::IsDown(const EAction& action)
	{
		for (IInputDevice* device : m_Devices)
		{
			if (device->IsDown(action))
			{
				return true;
			}
		}
		return false;
	}

	bool InputManager::OnDown(const EAction& action)
	{
		for (IInputDevice* device : m_Devices)
		{
			if (device->OnDown(action))
			{
				return true;
			}
		}
		return false;
	}

	bool InputManager::OnRelease(const EAction& action)
	{
		for (IInputDevice* device : m_Devices)
		{
			if (device->OnRelease(action))
			{
				return true;
			}
		}
		return false;
	}

	void InputManager::Update()
	{
		for (IInputDevice* device : m_Devices)
		{
			device->Update();
		}
	}

	const Input::Cursor& InputManager::GetCursor() const
	{
		return static_cast<InputDeviceMouse_Win32*>(m_Mouse)->GetCursor();
	}

	void InputManager::AddMapping(const EAction& action, const uint8 key, const DeviceType& device_type)
	{
		for (IInputDevice* device : m_Devices)
		{
			if (device->GetType() == device_type)
			{
				device->AddMapping(action, key);
			}
		}
	}

}; //namespace Input