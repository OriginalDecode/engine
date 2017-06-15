#include "BaseSystem.h"
#include "../Engine/Engine.h"

BaseSystem::BaseSystem(NodeEntityManager& anEntityManager)
	: m_Manager(anEntityManager)
	, m_Engine(Engine::GetInstance())
{
}

BaseSystem::BaseSystem(NodeEntityManager& anEntityManager, const ComponentFilter& aFilter)
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