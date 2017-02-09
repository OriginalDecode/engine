#pragma once
#include <standard_datatype.hpp>
#include <functional>
#include <map>
#include "../CommonLib/Math/Vector/Vector.h"
#include "../Input/InputWrapper.h"

#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif

class ControllerInput;
class InputWrapper;
class InputHandle
{
public:
	InputHandle() = default;
	bool Initiate(HWND window_handle, HINSTANCE window_instance);
	void AddController(u16 controller_id);
	void CleanUp();
	void HandleInput();
	void Bind(u32 hash, std::function<void()> function);


	void Update(float dt = 0.f);


	CU::Vector2f GetCursorPos() const { return m_Input->GetCursorPos(); }
	CU::Vector2f GetDeltaCursorPos() const { return { m_Input->MouseDirectX(), m_Input->MouseDirectY() }; }


	float GetX();
	float GetY();

	float GetDX();
	float GetDY();

	ControllerInput* GetController(u16 controller_id);
	InputWrapper* GetInputWrapper() { return m_Input; }

private:
	void CallFunction(u32 hash);
	static constexpr s32 m_ControllerMaxAmount = 8;
	s32 m_ControllerID;
	ControllerInput* m_Controller[m_ControllerMaxAmount];
	

	InputWrapper* m_Input = nullptr;
	std::map<u32, std::function<void()>> m_Functions;
};