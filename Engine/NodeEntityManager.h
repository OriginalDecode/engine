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

private:
	CU::GrowingArray<TreeDweller*> m_Entities;
	CU::GrowingArray<BaseSystem*> m_Systems;
	CComponentContainer m_Components;


};

