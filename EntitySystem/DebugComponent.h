#pragma once
#include "BaseComponent.h"
#include "../Physics/RigidBody.h"
#include "../Physics/GhostObject.h"


struct DebugComponent final : public BaseComponent
{
	~DebugComponent() { delete m_GhostObject; m_GhostObject = nullptr; }
	GhostObject* m_GhostObject = nullptr;


};