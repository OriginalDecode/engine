#pragma once
#include "EntityTypes.h"
#include "DataStructures/GrowingArray.h"
#include "DataStructures/StaticArray.h"

struct SComponentFilter;
struct BaseComponent;
class CComponentContainer
{
public:
	CComponentContainer();
	~CComponentContainer();

	void AddEntity();
	void AddComponent(Entity anEntity, BaseComponent* aComponent, unsigned int aComponentID);
	BaseComponent& GetComponent(Entity anEntity, unsigned int aComponentID);
	bool HasComponent(Entity anEntity, unsigned int aComponentID);

	const CU::GrowingArray<Entity>& GetEntities(SComponentFilter& aFilter);

private:
	CU::GrowingArray<ComponentArray> myComponents;
	CU::GrowingArray<EntityComponentArray> myEntityComponents;
	CU::GrowingArray<Entity> myEntitiesToReturn;

};

