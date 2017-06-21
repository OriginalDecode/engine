#pragma once
#include "BaseComponent.h"

class RigidBody;
struct PhysicsComponent : public BaseComponent
{
	~PhysicsComponent();
	RigidBody* myBody = nullptr;
	/* Data goes here */
	bool m_IsPlayer = false;
};

