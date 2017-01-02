#pragma once
#include "BaseSystem.h"

namespace Hex
{
	class Synchronizer;
}

class EntityManager;
class CRenderSystem : public BaseSystem
{
public:
	CRenderSystem(EntityManager& anEntityManager);

	void Update(float aDeltaTime) override;

private:
	Hex::Synchronizer* mySynchronizer;

};

