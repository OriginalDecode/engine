#pragma once
#include "EntityTypes.h"
#include "../CommonLib/DataStructures/GrowingArray.h"
#include "../CommonLib/DataStructures/StaticArray.h"

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

	//have filter with only 1 to check plz
	bool HasComponent(Entity e, SComponentFilter& filter);

	const CU::GrowingArray<Entity>& GetEntities(SComponentFilter& aFilter);

	void RemoveComponent(Entity entity, BaseComponent* component, u32 component_id);

private:
	CU::GrowingArray<ComponentArray> myComponents;
	CU::GrowingArray<EntityComponentArray> myEntityComponents;
	CU::GrowingArray<Entity> myEntitiesToReturn;

};

