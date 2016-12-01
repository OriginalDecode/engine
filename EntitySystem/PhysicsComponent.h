#pragma once
#include "BaseComponent.h"
class RigidBody;
struct SPhysicsComponent : public BaseComponent
{
	~SPhysicsComponent();
	RigidBody* myBody = nullptr;
	/* Data goes here */
} typedef PhysicsComponent;

