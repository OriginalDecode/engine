#pragma once
#include "BaseSystem.h"

namespace Hex
{
	class Synchronizer;
}

class EntityManager;
class RenderSystem : public BaseSystem
{
public:
	RenderSystem(EntityManager& anEntityManager);

	void Update(float dt) override;

private:
	Hex::Synchronizer* mySynchronizer;

};

