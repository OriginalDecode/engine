#pragma once
#include "../standard_datatype.hpp"
class InputCommand;
class ControllerInput;
class InputWrapper;

#define BIND_DECL(name) \
public : \
void Bind##name(InputCommand* input_command) { m_##name = input_command; } \
private : \
InputCommand * m_##name;


class InputHandle
{
public:
	InputHandle() = default;
	void Initiate(u16 controller_ID);
	void CleanUp();
	InputCommand* HandleInput();
	void Update();

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
};

#undef BIND_DECL