#pragma once
#include "EntityTypes.h"
#include "../CommonLib/DataStructures/GrowingArray.h"
#include "../CommonLib/DataStructures/StaticArray.h"

struct ComponentFilter;
struct BaseComponent;

struct EntityComponent
{
	Entity m_Entity;
	EntityComponentArray m_EntityArray;
	bool m_UpdateFlag = true;
};


class CComponentContainer
{
public:
	CComponentContainer(bool owner = false);
	~CComponentContainer();

	void AddEntity(Entity id);
	void AddComponent(Entity anEntity, BaseComponent* aComponent, unsigned int aComponentID);
	BaseComponent& GetComponent(Entity anEntity, unsigned int aComponentID);
	void SetUpdateFlag(Entity entity, bool flag);
	//have filter with only 1 to check plz
	bool HasComponent(Entity e, ComponentFilter filter);

	const CU::GrowingArray<Entity>& GetEntities(ComponentFilter aFilter);

	void RemoveComponent(Entity entity, BaseComponent* component, u32 component_id);

private:
	CU::GrowingArray<ComponentArray> myComponents;
	CU::GrowingArray<EntityComponent> myEntityComponents;
	CU::GrowingArray<Entity> myEntitiesToReturn;
	bool m_Owner = false;

};

