#include "PhysicsComponent.h"
#include "../Physics/RigidBody.h"
SPhysicsComponent::~SPhysicsComponent()
{
	delete myBody;
	myBody = nullptr;
	bool isPlayer = false;
}
