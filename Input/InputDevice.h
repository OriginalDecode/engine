#pragma once
#include <standard_datatype.hpp>
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")


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
		virtual bool OnDown(uint8 key) const = 0;
		virtual bool OnRelease(uint8 key) const = 0;
		virtual bool IsDown(uint8 key) const = 0;

		virtual void Update() = 0;

		const DeviceType& GetType() const { return m_Type; }

	protected:
		virtual void Release() = 0;
		DeviceType m_Type = DeviceType::NONE;
		IDirectInput8* m_Input = nullptr;
		IDirectInputDevice8* m_Device = nullptr;
	};
}; //namespace Input