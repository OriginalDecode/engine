#pragma once
#include "../standard_datatype.hpp"
class InputCommand;
class ControllerInput;
class InputWrapper;
class InputHandle
{
public:
	InputHandle() = default;
	void Initiate(u16 controller_ID);
	void CleanUp();
	InputCommand* HandleInput();


	void BindX(InputCommand* input_command) { m_XButton = input_command; }
	void BindY(InputCommand* input_command)	{ m_YButton = input_command; }
	void BindB(InputCommand* input_command)	{ m_BButton = input_command; }
	void BindA(InputCommand* input_command) { m_AButton = input_command; }

	void BindLTYP(InputCommand* input_command) { m_LThumbYP = input_command; }
	void BindLTYN(InputCommand* input_command) { m_LThumbYN = input_command; }
	void BindLTXP(InputCommand* input_command) { m_LThumbXP = input_command; }
	void BindLTXN(InputCommand* input_command) { m_LThumbXN = input_command; }

	void BindSpaceBar(InputCommand* input_command) { m_SpaceBar = input_command; }

private:
	ControllerInput* m_Controller = nullptr;
	InputWrapper* m_Input = nullptr;


	InputCommand* m_XButton = nullptr;
	InputCommand* m_YButton = nullptr;
	InputCommand* m_BButton = nullptr;

	InputCommand* m_AButton = nullptr;
	InputCommand* m_SpaceBar = nullptr;

	InputCommand* m_LThumbYP = nullptr;
	InputCommand* m_LThumbYN = nullptr;
	InputCommand* m_LThumbXP = nullptr;
	InputCommand* m_LThumbXN = nullptr;

};

