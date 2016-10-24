#pragma once
#include "ControllerComponent.h"
#include <standard_datatype.hpp>
class InputHandle;
struct InputController final : public ControllerComponent
{
	//~InputController() override { delete m_Controller; m_Controller = nullptr; }
	s16 m_ID = -1;
	InputHandle* m_InputHandle = nullptr;
	//ControllerInput* m_Controller = nullptr;
};