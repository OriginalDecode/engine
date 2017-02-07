#pragma once
#include "BaseSystem.h"
class EntityManager;
class PhysicsManager;

class PhysicsSystem : public BaseSystem 
{
public:
	PhysicsSystem(EntityManager& anEntityManager);

	void Update(float aDeltaTime) override;

private:
	PhysicsManager* myPhysicsManager;

	double myAccumulatedTime = 0.0;
};

