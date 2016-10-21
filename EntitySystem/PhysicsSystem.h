#pragma once
#include "BaseSystem.h"
class CEntityManager;
class CPhysicsManager;

class CPhysicsSystem : public BaseSystem 
{
public:
	CPhysicsSystem(CEntityManager& anEntityManager, CPhysicsManager* aPhysicsManager);

	void Update(float aDeltaTime) override;

private:
	CPhysicsManager* myPhysicsManager;

	double myAccumulatedTime = 0.0;
};

