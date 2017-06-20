#include "EntityManager.h"
#include "BaseSystem.h"
#include <EngineDefines.h>
#include "TranslationComponent.h"
#include "../Application/CameraHandle.h"
#include "NodeEntityManager.h"
#define TRUE 1
#define FALSE 0


void EntityManager::Initiate()
{
	myComponents = new CComponentContainer(true);
	m_NodeManagers.Init(m_MaxNodeCount); //should be a continous block of memory?
	for (s32 i = 0; i < m_MaxNodeCount; i++)
	{
		m_NodeManagers.Add(new NodeEntityManager);
	}

}

void EntityManager::CleanUp()
{
	for (NodeEntityManager* manager : m_NodeManagers)
	{
		manager->CleanUp();
		SAFE_DELETE(manager);
	}

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

void EntityManager::Update(float aDelta)
{
	/*myDeltaTime = aDelta;

	const CU::GrowingArray<Entity>& entities = GetEntities();
	for ( Entity e : entities )
	{
		if ( CameraHandle::GetInstance() )
		{
			TranslationComponent& t = GetComponent<TranslationComponent>(e);

			if ( CameraHandle::GetInstance()->GetFrustum().InsideAABB(t.myOrientation.GetPosition()) )
				myComponents->SetUpdateFlag(e, true);
			else
				myComponents->SetUpdateFlag(e, false);
		}
	}


	for ( BaseSystem* system :  )
	{
		system->Update(myDeltaTime);
	}*/
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

bool EntityManager::HasComponent(Entity e, ComponentFilter& filter)
{
	return myComponents->HasComponent(e, filter);
}

NodeEntityManager* EntityManager::RequestManager()
{
	for (s32 i = 0; i < m_MaxNodeCount; i++)
	{
		if (m_Systems[i] == 0)
		{
			m_Systems[i] = 1;
			return m_NodeManagers[i];
		}
	}
	DL_ASSERT("No free managers found, error!");
	return nullptr;
}

void EntityManager::ReleaseManager(NodeEntityManager* manager)
{
	const s32 index = m_NodeManagers.Find(manager);
	m_Systems[index] = 0;

}