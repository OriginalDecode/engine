#pragma once
#include "ComponentContainer.h"
#include "TypeID.h"
#include "EntityTypes.h"
#include <atomic>
#include <Engine/Octree.h>
#include "BaseComponent.h"
#include "../CommonLib/DataStructures/GrowingArray.h"
typedef CU::GrowingArray<Entity> EntityArray;
class NodeEntityManager;

class Synchronizer;
class PhysicsManager;
class BaseSystem;



class EntityManager
{
public:

	enum eSystemType
	{
		RENDER = BITFLAG(0),
		AI = BITFLAG(1),
		DEBUG = BITFLAG(2),
		PHYSICS = BITFLAG(3),
		LIGHT = BITFLAG(4),
	};

	EntityManager() = default;
	~EntityManager();
	void Initiate();

	void Reset();

	Entity CreateEntity();
	void Clear();
	void Update(float aDelta);
	const EntityArray& GetEntities(ComponentFilter aFilter);

	const EntityArray& GetEntities();

	template<typename T>
	T& AddComponent(Entity aEntity);

	template<typename T>
	void RemoveComponent(Entity aEntity, int aComponentID);

	template<typename T>
	T& GetComponent(Entity aEntity);

	float GetDeltaTime();

	void AddSystem(s32 type);
	s32 GetSystemFlag() const { return m_SystemsAdded; }

	template <typename T>
	void AddSystem();

	bool HasComponent(Entity e, ComponentFilter& filter);

	NodeEntityManager* RequestManager();
	void ReleaseManager(NodeEntityManager* manager);


	//CU::GrowingArray<BaseSystem*> GetSystems() { return mySystems; }

	//void SetActiveNodeManager(NodeEntityManager* manager) { m_ActiveNode = manager; }
	//NodeEntityManager* GetNodeManager() { return m_ActiveNode; }

	/*void RegisterManager(NodeEntityManager* manager);
	void UnRegisterManager(NodeEntityManager* manager);
	NodeEntityManager* GetManager(s32 index);*/

private:
	NodeEntityManager* m_ActiveNode = nullptr;

	CU::GrowingArray<NodeEntityManager*> m_NodeManagers;
	CU::GrowingArray<s32> m_UsedManagers;
	const s32 m_Max = (8 * 8 * 8 * 8 * 8);
	Entity myNextEntity = 0;
	CComponentContainer* myComponents = nullptr;
	CU::GrowingArray<BaseSystem*> m_Systems;
	std::atomic<float> myDeltaTime = 0.f;
	s32 m_SystemsAdded;

};

template<typename T>
T& EntityManager::AddComponent(Entity aEntity)
{
	T* component = new T();
	myComponents->AddComponent(aEntity, component, CTypeID<BaseComponent>::GetID<T>());
	return *component;
}

template<typename T>
void EntityManager::RemoveComponent(Entity aEntity, int aComponentID)
{
	myComponents->RemoveComponent(aEntity, CTypeID<BaseComponent>::GetID<T>(), aComponentID);
}

template<typename T>
T& EntityManager::GetComponent(Entity aEntity)
{
	return static_cast<T&>(myComponents->GetComponent(aEntity, CTypeID<BaseComponent>::GetID<T>()));
}

template <typename T>
void EntityManager::AddSystem()
{
		// 	for (NodeEntityManager* manager : m_NodeManagers)
		// 	{
		// 		manager->AddSystem<T>();
		// 	}
}