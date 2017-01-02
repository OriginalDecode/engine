#pragma once
#include "BaseSystem.h"


namespace Hex
{
	class Synchronizer;

};

class EntityManager;
class CLightSystem : public BaseSystem
{
public:
	CLightSystem(EntityManager& anEntityManager);
	void Update(float aDeltaTime) override;
private:
	Hex::Synchronizer* mySynchronizer;
};

