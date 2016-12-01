#pragma once
#include "BaseSystem.h"


namespace Snowblind
{
	class Synchronizer;

};

class CEntityManager;
class CLightSystem : public BaseSystem
{
public:
	CLightSystem(CEntityManager& anEntityManager, Snowblind::Synchronizer* aSynchronizer);
	void Update(float aDeltaTime) override;
private:
	Snowblind::Synchronizer* mySynchronizer;
};

