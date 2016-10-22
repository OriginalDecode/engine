#pragma once
#include "BaseComponent.h"
class CRigidBody;
struct SPhysicsComponent : public BaseComponent
{
	~SPhysicsComponent();
	CRigidBody* myBody = nullptr;
	bool m_IsPlayer = false;
	/* Data goes here */
} typedef PhysicsComponent;

