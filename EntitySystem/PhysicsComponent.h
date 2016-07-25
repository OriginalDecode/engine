#pragma once
#include "BaseComponent.h"
class CRigidBody;
struct SPhysicsComponent : public SBaseComponent
{
	~SPhysicsComponent();
	CRigidBody* myBody = nullptr;
	/* Data goes here */
} typedef PhysicsComponent;

