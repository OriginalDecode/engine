#pragma once
#include <standard_datatype.hpp>
#include <functional>
#include <map>

class ControllerInput;
class InputWrapper;
class InputHandle
{
public:
	InputHandle() = default;
	bool Initiate(u16 controller_ID);
	void CleanUp();
	void HandleInput();
	void Bind(u32 hash, std::function<void()> function);
	const ControllerInput& GetController() { return *m_Controller; }

	float GetX();
	float GetY();

	float GetDX();
	float GetDY();

private:
	void CallFunction(u32 hash);
	ControllerInput* m_Controller = nullptr;
	InputWrapper* m_Input = nullptr;
	std::map<u32, std::function<void()>> m_Functions;
};