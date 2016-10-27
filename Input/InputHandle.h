#pragma once
#include "../standard_datatype.hpp"
#include <functional>
#include <vector>
class InputCommand;
class ControllerInput;
class InputWrapper;

#define BIND_DECL(name)                                                        \
  \
public:                                                                        \
  \
void Bind##name(std::function<void()> input_command) {                         \
	m_##name = input_command;                                                  \
  }                                                                            \
  \
private:                                                                       \
  \
std::function<void()>                                                          \
	  m_##name;

class InputHandle
{
public:
	InputHandle() = default;
	void Initiate(u16 controller_ID);
	void CleanUp();
	void HandleInput();
	void Update();
	void Bind(u32 hash, std::function<void()> function);
	BIND_DECL(XButton);
	BIND_DECL(YButton);
	BIND_DECL(BButton);
	BIND_DECL(AButton);
	BIND_DECL(WKey);
	BIND_DECL(SKey);
	BIND_DECL(AKey);
	BIND_DECL(DKey);
	BIND_DECL(SpaceBar);
	BIND_DECL(LThumbYP);
	BIND_DECL(LThumbYN);
	BIND_DECL(LThumbXP);
	BIND_DECL(LThumbXN);
private:
	ControllerInput* m_Controller = nullptr;
	InputWrapper* m_Input = nullptr;
	std::vector<const char*> m_Names;
};

#undef BIND_DECL