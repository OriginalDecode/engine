#pragma once
#include "BaseSystem.h"


namespace Snowblind
{
	class CSynchronizer;

};

class CEntityManager;
class CLightSystem : public BaseSystem
{
public:
	CLightSystem(CEntityManager& anEntityManager, Snowblind::CSynchronizer* aSynchronizer);
	void Update(float aDeltaTime) override;
private:
	Snowblind::CSynchronizer* mySynchronizer;
};

