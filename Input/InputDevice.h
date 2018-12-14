#pragma once
#include <standard_datatype.hpp>
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")


struct IDirectInputDevice8A;
typedef IDirectInputDevice8A IDirectInputDevice8;

struct IDirectInput8A;
typedef IDirectInput8A IDirectInput8;

//This should be a pure virtual class
class IInputDevice
{
public:
	virtual ~IInputDevice() = 0;
	virtual bool OnDown(uint8 key) const = 0;
	virtual bool OnRelease(uint8 key) const = 0;
	virtual bool IsDown(uint8 key) const = 0;

	virtual void Update() = 0;

protected:
	IDirectInput8* m_Input = nullptr;
	IDirectInputDevice8* m_Device = nullptr;
	uint8 m_State[256];
	uint8 m_PrevState[256];
};