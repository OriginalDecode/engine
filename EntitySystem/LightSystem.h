#pragma once
#include "BaseSystem.h"



class Synchronizer;
class EntityManager;
class LightSystem : public BaseSystem
{
public:
	LightSystem(EntityManager& anEntityManager);
	void Update(float aDeltaTime) override;
private:
	Synchronizer* mySynchronizer;
};

