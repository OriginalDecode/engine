#pragma once
#include "BaseSystem.h"


namespace Hex
{
	class Synchronizer;

};

class EntityManager;
class LightSystem : public BaseSystem
{
public:
	LightSystem(EntityManager& anEntityManager);
	void Update(float aDeltaTime) override;
private:
	Hex::Synchronizer* mySynchronizer;
};

