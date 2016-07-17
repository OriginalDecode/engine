#include "EntityManager.h"
#include "BaseSystem.h"
#define TRUE 1
#define FALSE 0

CEntityManager::CEntityManager()
{
	myFinishedSystems.reset();

	for (int i = 0; i < MAX_COMPONENTS_COUNT; i++)
	{
		myFinishedSystems[i] = TRUE;
	}

	/* Create all Systems */
	/* Start all Systems */
	/* If a system is not needed, yield its thread */


}


CEntityManager::~CEntityManager()
{
}

Entity CEntityManager::CreateEntity()
{

}

void CEntityManager::Update(float aDelta)
{
	myDeltaTime = aDelta;
	
	/* Get Translation System & Update */
	while (IsSystemsFinished() == false){ }
	

}

const CU::GrowingArray<Entity>& CEntityManager::GetEntities(const SComponentFilter& aFilter)
{

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
