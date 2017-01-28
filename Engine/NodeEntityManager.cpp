#include "stdafx.h"
#include "NodeEntityManager.h"
#include <BaseSystem.h>
#include "Engine.h"
#include <EntityManager.h>
void NodeEntityManager::Initiate()
{
	m_Systems = Hex::Engine::GetInstance()->GetEntityManager().GetSystems();
}

void NodeEntityManager::AddEntity(TreeDweller* entity)
{
	m_Entities.Add(entity);
	ComponentList& list = entity->GetComponentPairList();
	Entity id = entity->GetEntity();
	m_Components.AddEntity(id);
	for (ComponentPair& pair : list)
	{
		m_Components.AddComponent(id, pair.m_Component, pair.m_ID);
	}
}

void NodeEntityManager::RemoveEntity(TreeDweller* entity)
{
	const ComponentList& list = entity->GetComponentPairList();
	Entity id = entity->GetEntity();
	for (const ComponentPair& pair : list)
	{
		m_Components.RemoveComponent(id, pair.m_Component, pair.m_ID);
	}
	m_Entities.RemoveCyclic(entity);
}

//void NodeEntityManager::AddSystem(BaseSystem* system)
//{
//	m_Systems.Add(system);
//}

void NodeEntityManager::Update(float dt)
{
	for (BaseSystem* system : m_Systems)
	{
		system->Update(dt);
	}
}

const CU::GrowingArray<Entity>& NodeEntityManager::GetEntities(SComponentFilter& filter)
{
	return m_Components.GetEntities(filter);
}
