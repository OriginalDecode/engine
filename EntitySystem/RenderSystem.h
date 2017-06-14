#pragma once
#include "BaseSystem.h"

class Synchronizer;
class RenderSystem : public BaseSystem
{
public:
	RenderSystem(NodeEntityManager& anEntityManager);

	void Update(float dt) override;

private:
	Synchronizer* mySynchronizer;

};

