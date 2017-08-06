#pragma once
#include "BaseSystem.h"



class Synchronizer;
class EntityManager;
class LightSystem : public BaseSystem
{
public:
	LightSystem(NodeEntityManager& anEntityManager);
	void Update(float aDeltaTime, bool paused) override;
private:
	Synchronizer* mySynchronizer;
};

