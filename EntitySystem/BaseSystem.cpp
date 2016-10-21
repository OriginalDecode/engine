#include "BaseSystem.h"



BaseSystem::BaseSystem(CEntityManager& anEntityManager)
	: myEntityManager(anEntityManager)
{
}


BaseSystem::BaseSystem(CEntityManager& anEntityManager, const SComponentFilter& aFilter)
	: myEntityManager(anEntityManager)
	, myFilter(aFilter)
{
}

BaseSystem::~BaseSystem() = default;

bool BaseSystem::HasFinished()
{
	return myHasFinished;
}

const CU::GrowingArray<Entity>& BaseSystem::GetEntities()
{
	return myEntityManager.GetEntities(myFilter);
}

