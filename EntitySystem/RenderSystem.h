#pragma once
#include "BaseSystem.h"

namespace Snowblind
{
	class CSynchronizer;
}

class CEntityManager;
class CRenderSystem : public BaseSystem
{
public:
	CRenderSystem(CEntityManager& anEntityManager, Snowblind::CSynchronizer* aSynchronizer);

	void Update(float aDeltaTime) override;

private:
	Snowblind::CSynchronizer* mySynchronizer;

};

