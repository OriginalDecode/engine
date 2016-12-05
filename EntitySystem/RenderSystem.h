#pragma once
#include "BaseSystem.h"

namespace Snowblind
{
	class Synchronizer;
}

class EntityManager;
class CRenderSystem : public BaseSystem
{
public:
	CRenderSystem(EntityManager& anEntityManager, Snowblind::Synchronizer* aSynchronizer);

	void Update(float aDeltaTime) override;

private:
	Snowblind::Synchronizer* mySynchronizer;

};

