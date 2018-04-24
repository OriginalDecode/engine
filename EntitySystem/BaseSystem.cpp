#include "BaseSystem.h"
#include "../Engine/Engine.h"

BaseSystem::BaseSystem(_EntityManager& anEntityManager)
	: m_Manager(anEntityManager)
	, m_Engine(Engine::GetInstance())
{
}

BaseSystem::BaseSystem(_EntityManager& anEntityManager, const ComponentFilter& aFilter)
	: m_Manager(anEntityManager)
	, myFilter(aFilter)
	, m_Engine(Engine::GetInstance())
{
}

BaseSystem::~BaseSystem() = default;

const CU::GrowingArray<Entity>& BaseSystem::GetEntities()
{
	return m_Manager.GetEntities(myFilter);
}