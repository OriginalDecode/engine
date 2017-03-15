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
	mySystems.ReInit(16);
	myFinishedSystems.reset();
	for ( int i = 0; i < MAX_COMPONENTS_COUNT; i++ )
	{
		myFinishedSystems[i] = TRUE;
	}

	myComponents = new CComponentContainer(true);
}

void EntityManager::CleanUp()
{
	for ( BaseSystem* system : mySystems )
	{
		SAFE_DELETE(system);
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
	myDeltaTime = aDelta;

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


	for ( BaseSystem* system : mySystems )
	{
		system->Update(myDeltaTime);
	}
}

const CU::GrowingArray<Entity>& EntityManager::GetEntities(SComponentFilter& aFilter)
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

bool EntityManager::HasComponent(Entity e, SComponentFilter& filter)
{
	return myComponents->HasComponent(e, filter);
}

void EntityManager::RegisterManager(NodeEntityManager* manager)
{
	m_RegisteredManagers.Add(manager);
}

void EntityManager::UnRegisterManager(NodeEntityManager* manager)
{
	m_RegisteredManagers.RemoveCyclic(manager);
}

NodeEntityManager* EntityManager::GetManager(s32 index)
{
	for (s32 i = 0; i < m_RegisteredManagers.Size(); i++)
	{
		if ( m_RegisteredManagers[i]->GetId() == index )
		{
			return m_RegisteredManagers[i];
		}
	}
	//emit warning
	return nullptr;
}

bool EntityManager::IsSystemsFinished()
{
	int count = 0;
	for ( int i = 0; i < mySystems.Size(); i++ )
	{
		if ( mySystems[i]->HasFinished() == true )
		{
			count++;
		}
	}

	if ( count < mySystems.Size() )
		return false;

	return true;
}
