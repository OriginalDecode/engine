#pragma once
#include "ComponentContainer.h"
#include "TypeID.h"
#include "EntityTypes.h"
#include "DataStructures/GrowingArray.h"
#include <atomic>
class CBaseSystem;
class CEntityManager
{
public:
	CEntityManager();
	~CEntityManager();

	Entity CreateEntity();
	void Update(float aDelta);
	const CU::GrowingArray<Entity>& GetEntities(const SComponentFilter& aFilter);

	template<typename T>
	void AddComponent(Entity aEntity);

	template<typename T>
	void RemoveComponent(Entity aEntity, int aComponentID);

	template<typename T>
	T& GetComponent(Entity aEntity);

	float GetDeltaTime();

	void OnFinish(); //Call this function??

private:
	Entity myNextEntity = 0;
	CComponentContainer* myComponents = nullptr;
	CU::GrowingArray<CBaseSystem*> mySystems;
	std::atomic<float> myDeltaTime = 0.f;
	std::atomic<bool> mySystemsFinished = false;

	std::bitset<MAX_COMPONENTS_COUNT> myFinishedSystems;
	short myNextFinish = 0;

	bool IsSystemsFinished();

};

template<typename T>
void CEntityManager::AddComponent(Entity aEntity)
{

}

template<typename T>
void CEntityManager::RemoveComponent(Entity aEntity, int aComponentID)
{

}

template<typename T>
T& CEntityManager::GetComponent(Entity aEntity)
{

}

