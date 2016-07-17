#include "ComponentContainer.h"
#include "ComponentFilter.h"


CComponentContainer::CComponentContainer()
	: myComponents(MAX_COMPONENTS_COUNT)
	, myEntityComponents(128)
	, myEntitiesToReturn(128)
{
	for (int i = 0; i < MAX_COMPONENTS_COUNT; i++)
	{
		myComponents.Add(ComponentArray());
		myComponents[i].Init(128);
	}
}

CComponentContainer::~CComponentContainer()
{
}

void CComponentContainer::AddEntity()
{
	myEntityComponents.Add(EntityComponentArray());
	EntityComponentArray& components = myEntityComponents.GetLast();
	for (int i = 0; i < MAX_COMPONENTS_COUNT; i++)
	{
		components[i] = -1;
	}
}

void CComponentContainer::AddComponent(Entity anEntity, CBaseComponent* aComponent, unsigned int aComponentID)
{
	myComponents[aComponentID].Add(aComponent);
	myEntityComponents[anEntity][aComponentID] = myComponents[aComponentID].Size() - 1;
}

CBaseComponent& CComponentContainer::GetComponent(Entity anEntity, unsigned int aComponentID)
{
	/*Error Handling*/

	int componentIndex = myEntityComponents[anEntity][aComponentID];
	return *myComponents[aComponentID][componentIndex];
}

bool CComponentContainer::HasComponent(Entity anEntity, unsigned int aComponentID)
{
	if (myEntityComponents.Size() <= anEntity)
		return false;
	return myEntityComponents[anEntity][aComponentID] != -1;
}

const CU::GrowingArray<Entity>& CComponentContainer::GetEntities(const SComponentFilter& aFilter)
{
	myEntitiesToReturn.RemoveAll();
	for (int i = 0; i < myEntityComponents.Size(); i++)
	{
		if (aFilter == myEntityComponents[i])
		{
			myEntitiesToReturn.Add(i);
		}
	}
	return myEntitiesToReturn;
}
