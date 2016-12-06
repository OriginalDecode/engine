#pragma once
#include "BaseSystem.h"
class EntityManager;
class PhysicsManager;

class CPhysicsSystem : public BaseSystem 
{
public:
	CPhysicsSystem(EntityManager& anEntityManager);

	void Update(float aDeltaTime) override;

private:
	PhysicsManager* myPhysicsManager;

	double myAccumulatedTime = 0.0;
};

