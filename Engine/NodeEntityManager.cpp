#include "stdafx.h"
#include "NodeEntityManager.h"
#include <BaseSystem.h>

void NodeEntityManager::AddEntity(TreeDweller* entity)
{
	m_Entities.Add(entity);
	const ComponentList& list = entity->GetComponentPairList();
	Entity id = entity->GetEntity();
	for (const ComponentPair& pair : list)
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

void NodeEntityManager::AddSystem(BaseSystem* system)
{
	m_Systems.Add(system);
}

void NodeEntityManager::Update(float dt)
{
	for (BaseSystem* system : m_Systems)
	{
		system->Update(dt);
	}
}
