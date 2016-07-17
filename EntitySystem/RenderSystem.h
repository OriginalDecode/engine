#pragma once
#include "BaseSystem.h"

namespace Snowblind
{
	class CSynchronizer;
}

class CEntityManager;
class CRenderSystem : public CBaseSystem
{
public:
	CRenderSystem(CEntityManager& anEntityManager, Snowblind::CSynchronizer* aSynchronizer);
	~CRenderSystem();

	void Update(float aDeltaTime) override;

private:
	Snowblind::CSynchronizer* mySynchronizer;

};

