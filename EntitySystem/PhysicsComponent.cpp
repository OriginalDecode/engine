#include "PhysicsComponent.h"
#include <Physics/RigidBody.h>
PhysicsComponent::~PhysicsComponent()
{
	delete myBody;
	myBody = nullptr;
}
