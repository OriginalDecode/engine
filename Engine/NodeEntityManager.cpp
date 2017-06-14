#include "stdafx.h"
#include "../Application/CameraHandle.h"
#include "NodeEntityManager.h"
#include <BaseSystem.h>
#include "Engine.h"
#include <EntityManager.h>
#include <TranslationComponent.h>

void NodeEntityManager::Initiate()
{

}

void NodeEntityManager::CleanUp()
{
	m_Systems.DeleteAll();
}

void NodeEntityManager::AddEntity(TreeDweller* entity)
{
	assert(entity->GetEntity() >= 0 && "entity was less than 0");
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

void NodeEntityManager::Update(float dt)
{
	/*const CU::GrowingArray<Entity>& entities = GetEntities(myFilter);
	if (entities.Size() > 0)
	{
		for (Entity e : entities)
		{
			CameraHandle* handle = CameraHandle::GetInstance();
			if (handle)
			{
				TranslationComponent& t = GetComponent<TranslationComponent>(e);

				if (CameraHandle::GetInstance()->GetFrustum().InsideAABB(t.myOrientation.GetPosition()))
					m_Components.SetUpdateFlag(e, true);
				else
					m_Components.SetUpdateFlag(e, false);
			}
		}
	}*/
	for (BaseSystem* system : m_Systems)
	{
		system->Update(dt);
	}
}

const CU::GrowingArray<Entity>& NodeEntityManager::GetEntities(ComponentFilter filter)
{
	return m_Components.GetEntities(filter);
}

NodeEntityManager::~NodeEntityManager()
{
	m_Entities.RemoveAll();
	m_Systems.RemoveAll();
}

bool NodeEntityManager::HasComponent(Entity e, ComponentFilter filter)
{
	return m_Components.HasComponent(e, filter);
}
