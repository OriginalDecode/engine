#pragma once
#include "BaseComponent.h"

class RigidBody;
struct PhysicsComponent : public BaseComponent
{
	~PhysicsComponent();
	RigidBody* m_Body = nullptr;

	void Serialize(JsonWriter& writer) const override;
	void Deserialize(const rapidjson::Value& value) override;


	/* Data goes here */
	bool m_IsPlayer = false;





};

