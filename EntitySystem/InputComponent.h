#pragma once
#include "ControllerComponent.h"
#include <standard_datatype.hpp>
#include "../Input/InputHandle.h"
struct InputController final : public ControllerComponent
{
	~InputController() override { delete m_InputHandle; m_InputHandle = nullptr; }
	s16 m_ID = -1;
	InputHandle* m_InputHandle = nullptr;
};