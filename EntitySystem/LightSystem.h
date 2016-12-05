#pragma once
#include "BaseSystem.h"


namespace Snowblind
{
	class Synchronizer;

};

class EntityManager;
class CLightSystem : public BaseSystem
{
public:
	CLightSystem(EntityManager& anEntityManager, Snowblind::Synchronizer* aSynchronizer);
	void Update(float aDeltaTime) override;
private:
	Snowblind::Synchronizer* mySynchronizer;
};

