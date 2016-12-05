#pragma once
#include "ComponentContainer.h"
#include "TypeID.h"
#include "EntityTypes.h"
#include "DataStructures/GrowingArray.h"
#include <atomic>
#include "BaseComponent.h"
namespace Snowblind
{
	class Synchronizer;
}
class PhysicsManager;
class BaseSystem;
class EntityManager
{
public:
	EntityManager() = default;

	void Initiate();
	void CleanUp();


	Entity CreateEntity();
	void Clear();
	void Update(float aDelta);
	const CU::GrowingArray<Entity>& GetEntities(SComponentFilter& aFilter);

	template<typename T>
	void AddComponent(Entity aEntity);

	template<typename T>
	void RemoveComponent(Entity aEntity, int aComponentID);

	template<typename T>
	T& GetComponent(Entity aEntity);

	float GetDeltaTime();

	template <typename T>
	void AddSystem();

	template <typename T>
	void AddSystem(Snowblind::Synchronizer* aSynchronizer);

	template <typename T>
	void AddSystem(PhysicsManager* aManager);

private:
	Entity myNextEntity = 0;
	CComponentContainer* myComponents = nullptr;
	CU::GrowingArray<BaseSystem*> mySystems;
	std::atomic<float> myDeltaTime = 0.f;
	std::atomic<bool> mySystemsFinished = false;

	std::bitset<MAX_COMPONENTS_COUNT> myFinishedSystems;
	short myNextFinish = 0;

	bool IsSystemsFinished();

};

template<typename T>
void EntityManager::AddComponent(Entity aEntity)
{
	T* component = new T();
	myComponents->AddComponent(aEntity, component, CTypeID<BaseComponent>::GetID<T>());
}

template<typename T>
void EntityManager::RemoveComponent(Entity aEntity, int aComponentID)
{

}

template<typename T>
T& EntityManager::GetComponent(Entity aEntity)
{
	return static_cast<T&>(myComponents->GetComponent(aEntity, CTypeID<BaseComponent>::GetID<T>()));
}

template <typename T>
void EntityManager::AddSystem()
{
	mySystems.Add(new T(*this));
}

template <typename T>
void EntityManager::AddSystem(Snowblind::Synchronizer* aSynchronizer)
{
	mySystems.Add(new T(*this, aSynchronizer));
}

template <typename T>
void EntityManager::AddSystem(PhysicsManager* aManager)
{
	mySystems.Add(new T(*this, aManager));
}
