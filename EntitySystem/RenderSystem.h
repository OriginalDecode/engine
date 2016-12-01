#pragma once
#include "BaseSystem.h"

namespace Snowblind
{
	class Synchronizer;
}

class CEntityManager;
class CRenderSystem : public BaseSystem
{
public:
	CRenderSystem(CEntityManager& anEntityManager, Snowblind::Synchronizer* aSynchronizer);

	void Update(float aDeltaTime) override;

private:
	Snowblind::Synchronizer* mySynchronizer;

};

