#pragma once
#include "BaseComponent.h"
class CRigidBody;
struct SPhysicsComponent : public SBaseComponent
{
	CRigidBody* myBody = nullptr;
	/* Data goes here */
};

