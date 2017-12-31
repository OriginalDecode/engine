#include "PhysicsComponent.h"
#include <Physics/RigidBody.h>
PhysicsComponent::~PhysicsComponent()
{
	delete m_Body;
	m_Body = nullptr;
}

void PhysicsComponent::Serialize(JsonWriter& writer) const
{
	assert(false && "not implemented!");
}

void PhysicsComponent::Deserialize(const rapidjson::Value& value)
{
	assert(false && "not implemented!");
}
