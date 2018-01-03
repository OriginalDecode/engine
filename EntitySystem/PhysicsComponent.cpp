#include "PhysicsComponent.h"
#include <Physics/RigidBody.h>
PhysicsComponent::~PhysicsComponent()
{
	delete m_Body;
	m_Body = nullptr;
}

void PhysicsComponent::Serialize(JsonWriter& writer) const
{
	m_Body->Serialize(writer);
}

void PhysicsComponent::Deserialize(const rapidjson::Value& value)
{
	m_Body = new RigidBody;
	m_Body->Deserialize(value);
}
