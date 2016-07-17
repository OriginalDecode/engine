#include "EntityManager.h"
#include "BaseSystem.h"
#include "EngineDefines.h"
#define TRUE 1
#define FALSE 0

CEntityManager::CEntityManager()
{
	mySystems.Init(16);
	myFinishedSystems.reset();
	for (int i = 0; i < MAX_COMPONENTS_COUNT; i++)
	{
		myFinishedSystems[i] = TRUE;
	}

	myComponents = new CComponentContainer();
}


CEntityManager::~CEntityManager()
{
	SAFE_DELETE(myComponents);
}

Entity CEntityManager::CreateEntity()
{
	Entity newEntity = myNextEntity++;

	myComponents->AddEntity();

	return newEntity;
}

void CEntityManager::Update(float aDelta)
{
	myDeltaTime = aDelta;
	
	
	for each(CBaseSystem* system in mySystems)
	{
		system->Update(myDeltaTime);
	}

	//while (IsSystemsFinished() == false){ }
	

}

const CU::GrowingArray<Entity>& CEntityManager::GetEntities(SComponentFilter& aFilter)
{
	return myComponents->GetEntities(aFilter);
}

float CEntityManager::GetDeltaTime()
{
	return myDeltaTime;
}

bool CEntityManager::IsSystemsFinished()
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
