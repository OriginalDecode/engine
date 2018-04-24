#pragma once
#include "BaseSystem.h"
class EntityManager;
class PhysicsManager;

class PhysicsSystem : public BaseSystem 
{
public:
	PhysicsSystem(_EntityManager& anEntityManager);

	void Update(float dt, bool paused) override;

private:
	PhysicsManager* myPhysicsManager;

	double myAccumulatedTime = 0.0;
};

