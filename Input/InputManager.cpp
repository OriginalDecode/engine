#include "InputManager.h"
#include "InputDevice.h"

#include <dinput.h>

#include "InputDeviceKeyboard_Win32.h"

namespace Input
{
	InputManager::InputManager(HWND window_handle, HINSTANCE window_instance)
	{
		AddMapping(A_MOVE_FORWARD, DIK_W);
		AddMapping(A_MOVE_BACK, DIK_S);
		AddMapping(A_MOVE_LEFT, DIK_A);
		AddMapping(A_MOVE_RIGHT, DIK_D);
		AddMapping(A_MOVE_UP, DIK_SPACE);
		AddMapping(A_MOVE_DOWN, DIK_X);

		m_Devices.emplace_back(new InputDeviceKeyboard_Win32(window_handle, window_instance));
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
			for (uint8 key : m_ActionMapping[action])
			{
				if (device->IsDown(key))
				{
					return true;
				}
			}
		}
		return false;
	}

	bool InputManager::OnDown(const EAction& action)
	{
		for (IInputDevice* device : m_Devices)
		{
			for (uint8 key : m_ActionMapping[action])
			{
				if (device->OnDown(key))
				{
					return true;
				}
			}
		}
		return false;
	}

	bool InputManager::OnRelease(const EAction& action)
	{
		for (IInputDevice* device : m_Devices)
		{
			for (uint8 key : m_ActionMapping[action])
			{
				if (device->OnRelease(key))
				{
					return true;
				}
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

	void InputManager::AddMapping(const EAction& action, const uint8 key)
	{
		auto placed = m_ActionMapping.emplace(action, std::vector<uint8>());
		placed.first->second.push_back(key);
	}

}; //namespace Input