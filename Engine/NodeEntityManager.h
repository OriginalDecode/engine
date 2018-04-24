#pragma once
#include "../EntitySystem/ComponentContainer.h"
#include "TreeDweller.h"

class BaseSystem;
class TreeNodeBase;
class NodeEntityManager
{
public:
	NodeEntityManager(TreeNodeBase* node)
		: m_Node(node)
	{
		m_ID = m_Identifier++;
	}
	~NodeEntityManager();


	void Initiate();
	void CleanUp();
	void AddEntity(TreeDweller* entity);
	void RemoveEntity(TreeDweller* entity);

	void AddComponent(TreeDweller* e);
	//void AddSystem(BaseSystem* system);

	void Update(float dt, const CU::GrowingArray<TreeDweller*>& dweller_list, bool paused);

	const CU::GrowingArray<Entity>& GetEntities(ComponentFilter filter);
	s32 GetId() const { return m_Identifier; }

	bool HasComponent(Entity e, ComponentFilter filter);

	template<typename T>
	T& GetComponent(Entity aEntity);

	template<typename T>
	void AddSystem();

	s32 EntityCount() {
		return m_Entities.Size();
	}

	

	void SetMemoryBlockIndex(s32 index);
	s32 GetMemoryBlockIndex() const { return m_MemoryBlockIndex; }
	TreeNodeBase* GetTreeNode() { return m_Node; }
private:
	static s32 m_Identifier;
	TreeNodeBase* m_Node = nullptr;
	s32 m_MemoryBlockIndex = 0;

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
#ifdef _PER_NODE_SYSTEM
	m_Systems.Add(new T(*this));
#endif
}