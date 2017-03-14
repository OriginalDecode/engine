#pragma once
#include <DataStructures/GrowingArray.h>
#include "../EntitySystem/ComponentContainer.h"
#include "TreeDweller.h"
class BaseSystem;
class NodeEntityManager
{
public:
	NodeEntityManager() = default;
	void Initiate();
	void AddEntity(TreeDweller* entity);
	void RemoveEntity(TreeDweller* entity);
	//void AddSystem(BaseSystem* system);

	void Update(float dt);

	const CU::GrowingArray<Entity>& GetEntities(SComponentFilter& filter);
	~NodeEntityManager();
private:
	CU::GrowingArray<TreeDweller*> m_Entities;
	CU::GrowingArray<BaseSystem*> m_Systems;
	CComponentContainer m_Components;
	template<typename T>
	T& GetComponent(Entity aEntity);
};

template<typename T>
T& NodeEntityManager::GetComponent(Entity aEntity)
{
	return static_cast< T& >( m_Components.GetComponent(aEntity, CTypeID<BaseComponent>::GetID<T>()) );
}

