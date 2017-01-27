#include "ComponentContainer.h"
#include "ComponentFilter.h"
#include "BaseComponent.h"

CComponentContainer::CComponentContainer()
	: myComponents(MAX_COMPONENTS_COUNT)
	, myEntityComponents(128)
	, myEntitiesToReturn(128)
{
	for (int i = 0; i < MAX_COMPONENTS_COUNT; i++)
	{
		myComponents.Add(ComponentArray());
		myComponents[i].ReInit(128);
	}
}

CComponentContainer::~CComponentContainer()
{
	for (int i = 0; i < myComponents.Size(); i++)
	{
		myComponents[i].DeleteAll();
	}
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

void CComponentContainer::AddComponent(Entity anEntity, BaseComponent* aComponent, unsigned int aComponentID)
{
	myComponents[aComponentID].Add(aComponent);
	myEntityComponents[anEntity][aComponentID] = myComponents[aComponentID].Size() - 1;
}

BaseComponent& CComponentContainer::GetComponent(Entity anEntity, unsigned int aComponentID)
{
	/*Error Handling*/

	int componentIndex = myEntityComponents[anEntity][aComponentID];
	return *myComponents[aComponentID][componentIndex];
}

bool CComponentContainer::HasComponent(Entity e, SComponentFilter& filter)
{
	if (filter.Compare(myEntityComponents[e]) == true)
	{
		return true;
	}
}

const CU::GrowingArray<Entity>& CComponentContainer::GetEntities(SComponentFilter& aFilter)
{
	myEntitiesToReturn.RemoveAll();
	for (int i = 0; i < myEntityComponents.Size(); i++)
	{
		if (aFilter.Compare(myEntityComponents[i]) == true)
		{
			myEntitiesToReturn.Add(i);
		}
	}
	return myEntitiesToReturn;
}

void CComponentContainer::RemoveComponent(Entity entity, BaseComponent* component, u32 component_id)
{
	myEntityComponents[entity][component_id]--;
	myComponents[component_id].RemoveCyclic(component);
}
