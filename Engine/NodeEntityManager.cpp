#include "stdafx.h"
#include "../Application/CameraHandle.h"
#include "NodeEntityManager.h"
#include <BaseSystem.h>
#include "Engine.h"
#include <EntityManager.h>
#include <TranslationComponent.h>
#include <Engine/Synchronizer.h>
s32 NodeEntityManager::m_Identifier = 0;

void NodeEntityManager::Initiate()
{

}

void NodeEntityManager::CleanUp()
{
	m_Systems.DeleteAll();
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

void NodeEntityManager::Update(float dt)
{
	const CU::GrowingArray<Entity>& entities = GetEntities(CreateFilter<Requires<TranslationComponent>>());
	for (s32 i = 0; i < entities.Size(); i++)
	{
		Entity e = entities[i];
		CameraHandle* handle = CameraHandle::GetInstance();
		if ( handle)
		{
			TranslationComponent& t = GetComponent<TranslationComponent>(e);

			if ( CameraHandle::GetInstance()->GetFrustum().InsideAABB(t.myOrientation.GetPosition()) )
				m_Components.SetUpdateFlag(e, true);
			else
				m_Components.SetUpdateFlag(e, false);
		}
	}

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

void* NodeEntityManager::GetModelCommandBlock()
{
	u16 current_buffer = Engine::GetInstance()->GetSynchronizer()->GetCurrentBuffer();
	return m_ModelCommandBlock[current_buffer];
}

void NodeEntityManager::SetModelCommandBlock(void* block0, void* block1)
{
	m_ModelCommandBlock[0] = block0;
	m_ModelCommandBlock[1] = block1;
}
