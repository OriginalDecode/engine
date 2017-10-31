#include "stdafx.h"
#include "../Application/CameraHandle.h"
#include "NodeEntityManager.h"
#include <BaseSystem.h>
#include "Engine.h"
#include <EntityManager.h>
#include <TranslationComponent.h>
#include <Engine/Synchronizer.h>

#include <EntitySystem/RenderSystem.h>
#include <EntitySystem/DebugSystem.h>
#include <EntitySystem/LightSystem.h>
#include <EntitySystem/PhysicsSystem.h>

void NodeEntityManager::SetMemoryBlockIndex(s32 index)
{
	m_MemoryBlockIndex = index;
}

s32 NodeEntityManager::m_Identifier = 0;

void NodeEntityManager::Initiate()
{
	EntityManager& em = Engine::GetInstance()->GetEntityManager();
	const s32 flag = em.GetSystemFlag();

	if (flag & EntityManager::RENDER)
		AddSystem<RenderSystem>();
	if (flag & EntityManager::PHYSICS)
		AddSystem<PhysicsSystem>();
	if (flag & EntityManager::LIGHT)
		AddSystem<LightSystem>();
#if !defined(_FINAL) && !defined(_PROFILE)
	if (flag & EntityManager::DEBUG)
		AddSystem<DebugSystem>();
#endif
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

void NodeEntityManager::Update(float dt, const CU::GrowingArray<TreeDweller*>& dweller_list, bool paused)
{
	PROFILE_FUNCTION(profiler::colors::Red);

	PROFILE_BLOCK("Update Entities", profiler::colors::Green);
	for (TreeDweller* dweller : dweller_list)
	{
		Entity e = dweller->GetEntity();
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
	PROFILE_BLOCK_END;
	for (BaseSystem* system : m_Systems)
	{
		system->Update(dt, paused);
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
