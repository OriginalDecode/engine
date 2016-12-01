#pragma once
#include "ComponentFilter.h"
#include "EntityManager.h"

namespace Snowblind
{
	class Synchronizer;
}

class BaseSystem
{
public:
	BaseSystem(CEntityManager& anEntityManager);
	BaseSystem(CEntityManager& anEntityManager, const SComponentFilter& aFilter);
	virtual ~BaseSystem() = 0;

	virtual void Update(float aDeltaTime) = 0;

	bool HasFinished();

	const CU::GrowingArray<Entity>& GetEntities();

	template<typename T>
	T& GetComponent(Entity anEntity);


private:
	void operator=(BaseSystem&) = delete;
	bool myHasFinished = false;
	CEntityManager& myEntityManager;
	SComponentFilter myFilter;

};

template<typename T>
T& BaseSystem::GetComponent(Entity anEntity)
{
	return myEntityManager.GetComponent<T>(anEntity);
}

