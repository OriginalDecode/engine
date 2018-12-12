#include "EntityManager.h"
#include "BaseSystem.h"
#include <EngineDefines.h>
#include "TranslationComponent.h"
#include "../Application/CameraHandle.h"
#include "NodeEntityManager.h"

#include "DebugSystem.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "LightSystem.h"
#include "InputSystem.h"
#include "AISystem.h"
#include "NetworkSystem.h"
#include "CameraSystem.h"


EntityManager::~EntityManager()
{
	m_NodeManagers.DeleteAll();
	SAFE_DELETE(myComponents);
}

void EntityManager::Initiate()
{
	myComponents = new CComponentContainer(true);
// 	m_NodeManagers.Init(m_MaxNodeCount); //should be a continous block of memory?
// 	for (s32 i = 0; i < m_MaxNodeCount; i++)
// 	{
// 		m_NodeManagers.Add(new NodeEntityManager);
// 	}

}

void EntityManager::Reset()
{
	myNextEntity = 0;
	SAFE_DELETE(myComponents);
}

Entity EntityManager::CreateEntity()
{
	Entity newEntity = myNextEntity++;
	myComponents->AddEntity(newEntity);

	return newEntity;
}

void EntityManager::Clear()
{
	SAFE_DELETE(myComponents);
	myNextEntity = 0;
}

void EntityManager::Update(float dt)
{

	//const CU::GrowingArray<Entity>& entities = GetEntities();
	
	/*for ( Entity e : entities )
	{
		if ( CameraHandle::GetInstance() )
		{
			TranslationComponent& t = GetComponent<TranslationComponent>(e);

			if ( CameraHandle::GetInstance()->GetFrustum().InsideAABB(t.myOrientation.GetPosition()) )
				myComponents->SetUpdateFlag(e, true);
			else
				myComponents->SetUpdateFlag(e, false);
		}
	}*/


	for ( BaseSystem* system : m_Systems )
	{
		system->Update(dt, false);
	}
}

const CU::GrowingArray<Entity>& EntityManager::GetEntities(ComponentFilter aFilter)
{
	return myComponents->GetEntities(aFilter);
}

const CU::GrowingArray<Entity>& EntityManager::GetEntities()
{
	return GetEntities(CreateFilter<Requires<TranslationComponent>>());
}

float EntityManager::GetDeltaTime()
{
	return myDeltaTime;
}

void EntityManager::AddSystem(int32 type)
{
	m_SystemsAdded |= type;

#ifndef _PER_NODE_SYSTEM
	if (type & eSystemType::RENDER)
		AddSystem<RenderSystem>();

	if (type & eSystemType::AI)
		AddSystem<AISystem>();

	if (type & eSystemType::PHYSICS)
		AddSystem<PhysicsSystem>();

#if !defined(_FINAL) && !defined(_PROFILE)
	if (type & eSystemType::DEBUG)
		AddSystem<DebugSystem>();
#endif

	if (type & eSystemType::LIGHT)
		AddSystem<LightSystem>();

	if (type & eSystemType::NETWORK)
		AddSystem<NetworkSystem>();
#endif
}

bool EntityManager::HasComponents(Entity e, ComponentFilter& filter)
{
	return myComponents->HasComponent(e, filter);
}

NodeEntityManager* EntityManager::RequestManager(TreeNodeBase* node)
{
	for (int32 i = 0; i < m_UsedManagers.Size(); i++)
	{
		if (m_UsedManagers[i] == 0)
		{
			m_UsedManagers[i] = 1;
			return m_NodeManagers[i];
		}
	}

	if (m_Systems.Size() < m_Max)
	{
		m_NodeManagers.Add(new NodeEntityManager(node));
		m_UsedManagers.Add(0);
		m_UsedManagers.GetLast() = 1;
		return m_NodeManagers.GetLast();

	}



	DL_ASSERT("No free managers found, error!");
	return nullptr;
}

void EntityManager::ReleaseManager(NodeEntityManager* manager)
{
	const int32 index = m_NodeManagers.Find(manager);
	m_UsedManagers[index] = 0;



	/*for (s32 i = 0; i < m_NodeManagers.Size(); i++)
	{
		const NodeEntityManager& ec = m_NodeManagers[i];
		if (ec.GetId() == manager.GetId())
		{
			m_Systems[i] = 0;
			break;
		}
	}*/

}