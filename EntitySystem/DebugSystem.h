#pragma once
#include "BaseSystem.h"

class DebugSystem : public BaseSystem
{
public:
	DebugSystem(EntityManager& entity_manager);
	void Update(float dt) override;
private:
	PhysicsManager* m_PhysicsManager;
	double m_AccumulatedTime = 0.0f;
};