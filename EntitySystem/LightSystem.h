#pragma once
#include "BaseSystem.h"


namespace Snowblind
{
	class CSynchronizer;

};

class CEntityManager;
class CLightSystem : public CBaseSystem
{
public:
	CLightSystem(CEntityManager& anEntityManager, Snowblind::CSynchronizer* aSynchronizer);
	~CLightSystem();
	void Update(float aDeltaTime) override;
private:
	Snowblind::CSynchronizer* mySynchronizer;
};

