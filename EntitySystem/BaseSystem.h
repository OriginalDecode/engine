#pragma once
#include "ComponentFilter.h"
#include "EntityManager.h"
#include <Utilities.h>
namespace Hex
{
	class Synchronizer;
}

class BaseSystem
{
public:
	BaseSystem(EntityManager& anEntityManager);
	BaseSystem(EntityManager& anEntityManager, const SComponentFilter& aFilter);
	virtual ~BaseSystem() = 0;

	virtual void Update(float aDeltaTime) = 0;

	bool HasFinished();

	const CU::GrowingArray<Entity>& GetEntities();

	template<typename T>
	T& GetComponent(Entity anEntity);

private:
	void operator=(BaseSystem&) = delete;
	bool myHasFinished = false;
	SComponentFilter myFilter;

protected:
	EntityManager& myEntityManager;
	Ticket_Mutex m_Mutex;
};

template<typename T>
T& BaseSystem::GetComponent(Entity anEntity)
{
	return myEntityManager.GetComponent<T>(anEntity);
}

