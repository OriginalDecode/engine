#include "EntityManager.h"
#include "BaseSystem.h"
#include <EngineDefines.h>
#include "TranslationComponent.h"
#define TRUE 1
#define FALSE 0

void EntityManager::Initiate()
{
	mySystems.ReInit(16);
	myFinishedSystems.reset();
	for (int i = 0; i < MAX_COMPONENTS_COUNT; i++)
	{
		myFinishedSystems[i] = TRUE;
	}

	myComponents = new CComponentContainer;
}

void EntityManager::CleanUp()
{
	for (BaseSystem* system : mySystems)
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
	
	for (BaseSystem* system : mySystems)
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

bool EntityManager::IsSystemsFinished()
{
	int count = 0;
	for (int i = 0; i < mySystems.Size(); i++)
	{
		if (mySystems[i]->HasFinished() == true)
		{
			count++;
		}
	}

	if (count < mySystems.Size())
		return false;

	return true;
}
