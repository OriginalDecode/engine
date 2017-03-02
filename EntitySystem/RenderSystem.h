#pragma once
#include "BaseSystem.h"


class Synchronizer;
class EntityManager;
class RenderSystem : public BaseSystem
{
public:
	RenderSystem(EntityManager& anEntityManager);

	void Update(float dt) override;

private:
	Synchronizer* mySynchronizer;

};

