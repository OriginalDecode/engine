#include "BaseSystem.h"

#include "../Engine/NodeEntityManager.h"
#include "../Engine/Engine.h"
BaseSystem::BaseSystem(EntityManager& anEntityManager)
	: myEntityManager(anEntityManager)
	, m_Engine(Engine::GetInstance())
{
}


BaseSystem::BaseSystem(EntityManager& anEntityManager, const SComponentFilter& aFilter)
	: myEntityManager(anEntityManager)
	, myFilter(aFilter)
	, m_Engine(Engine::GetInstance())
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


	return myEntityManager.GetNodeManager()->GetEntities(myFilter);
}