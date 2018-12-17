#pragma once
#include <standard_datatype.hpp>
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include "InputActions.h"

#include <unordered_map>
#include <vector>

struct IDirectInputDevice8A;
typedef IDirectInputDevice8A IDirectInputDevice8;

struct IDirectInput8A;
typedef IDirectInput8A IDirectInput8;


//This should be a pure virtual class
namespace Input
{
	enum DeviceType
	{
		NONE,
		KEYBOARD,
		MOUSE,
		GAMEPAD,
	};

	class IInputDevice
	{
	public:
		virtual ~IInputDevice() { };
		virtual bool OnDown(const EAction& action) const = 0;
		virtual bool OnRelease(const EAction& action) const = 0;
		virtual bool IsDown(const EAction& action) const = 0;

		virtual void Update() = 0;

		const DeviceType& GetType() const { return m_Type; }

		void AddMapping(const EAction& action, const uint8 key)
		{
			auto placed = m_ActionMapping.emplace(action, std::vector<uint8>());
			placed.first->second.push_back(key);
		}

	protected:
		virtual void Release() = 0;
		DeviceType m_Type = DeviceType::NONE;
		IDirectInput8* m_Input = nullptr;
		IDirectInputDevice8* m_Device = nullptr;

		std::unordered_map<EAction, std::vector<uint8>> m_ActionMapping;

	};
}; //namespace Input