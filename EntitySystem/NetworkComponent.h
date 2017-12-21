#pragma once
#include "ControllerComponent.h"

struct NetworkComponent final: public ControllerComponent
{
	GUID m_GUID;
	GUID m_Owner;


};