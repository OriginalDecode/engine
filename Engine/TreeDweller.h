#pragma once
#include "snowblind_shared.h"
#include "../EntitySystem/BaseComponent.h"
#include <DataStructures/GrowingArray.h>

struct ComponentPair
{
	BaseComponent* m_Component = nullptr;
	s32 m_ID = -1;
};

typedef CU::GrowingArray<ComponentPair> ComponentList;

class TreeDweller
{
public:
	TreeDweller() = default;
	void Initiate(Entity entity);

	template <typename T>
	void AddComponent(T* component);


	Entity GetEntity() const;
	ComponentList& GetComponentPairList();

private:
	Entity m_Entity;
	ComponentList m_ComponentList;
};

template <typename T>
void TreeDweller::AddComponent(T* component)
{
	ComponentPair pair;
	pair.m_Component = component;
	pair.m_ID = CTypeID<BaseComponent>::GetID<T>();
	m_ComponentList.Add(pair);
}