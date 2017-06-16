#pragma once
#include "../EntitySystem/ComponentContainer.h"
#include "TreeDweller.h"
class BaseSystem;
class NodeEntityManager
{
public:
	NodeEntityManager()
	{
		m_ID = m_Identifier++;
	}
	void Initiate();
	void CleanUp();
	void AddEntity(TreeDweller* entity);
	void RemoveEntity(TreeDweller* entity);
	//void AddSystem(BaseSystem* system);

	void Update(float dt);

	const CU::GrowingArray<Entity>& GetEntities(ComponentFilter filter);
	~NodeEntityManager();
	s32 GetId() { return m_Identifier; }

	bool HasComponent(Entity e, ComponentFilter filter);

	template<typename T>
	T& GetComponent(Entity aEntity);

	template<typename T>
	void AddSystem();
private:
	static s32 m_Identifier;
	s32 m_ID = 0;
	CU::GrowingArray<TreeDweller*> m_Entities;
	CU::GrowingArray<BaseSystem*> m_Systems;
	CComponentContainer m_Components;
	
};

template<typename T>
T& NodeEntityManager::GetComponent(Entity aEntity)
{
	return static_cast< T& >( m_Components.GetComponent(aEntity, CTypeID<BaseComponent>::GetID<T>()) );
}

template <typename T>
void NodeEntityManager::AddSystem()
{
	m_Systems.Add(new T(*this));
}