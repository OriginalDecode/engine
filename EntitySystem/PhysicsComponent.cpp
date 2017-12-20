#include "PhysicsComponent.h"
#include <Physics/RigidBody.h>
PhysicsComponent::~PhysicsComponent()
{
	delete m_Body;
	m_Body = nullptr;
}
