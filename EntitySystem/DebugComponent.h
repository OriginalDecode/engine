#pragma once
#include "BaseComponent.h"
#include "../Physics/RigidBody.h"

struct DebugComponent final : public BaseComponent
{
	~DebugComponent() { delete m_Body; m_Body = nullptr; }
	RigidBody* m_Body = nullptr;


};