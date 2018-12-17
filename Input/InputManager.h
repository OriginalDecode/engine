#pragma once
#include <standard_datatype.hpp>

#include "InputActions.h"
#include "InputDevice.h"

#include <unordered_map>
#include <vector>

#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif

namespace Input
{
	class IInputDevice;

	struct Cursor;

	class InputManager
	{
	public:
		InputManager(HWND window_handle, HINSTANCE window_instance);
		~InputManager();

		bool IsDown(const EAction& action);
		bool OnDown(const EAction& action);
		bool OnRelease(const EAction& action);

		void Update();
		
		const Cursor& GetCursor() const;

	private:

		void AddMapping(const EAction& action, const uint8 key, const DeviceType& device_type);
		std::vector<IInputDevice*> m_Devices;
		IInputDevice* m_Mouse = nullptr;


	};

}; //namespace Input
