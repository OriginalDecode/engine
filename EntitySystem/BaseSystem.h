#pragma once
#include "ComponentFilter.h"
#include "EntityManager.h"

namespace Snowblind
{
	class CSynchronizer;
}

class CBaseSystem
{
public:
	CBaseSystem(CEntityManager& anEntityManager);
	CBaseSystem(CEntityManager& anEntityManager, const SComponentFilter& aFilter);
	virtual ~CBaseSystem();

	virtual void Update(float aDeltaTime) = 0;

	bool HasFinished();

	const CU::GrowingArray<Entity>& GetEntities();

	template<typename T>
	T& GetComponent(Entity anEntity);


private:
	void operator=(CBaseSystem&) = delete;
	bool myHasFinished = false;
	CEntityManager& myEntityManager;
	SComponentFilter myFilter;

};

template<typename T>
T& CBaseSystem::GetComponent(Entity anEntity)
{
	return myEntityManager.GetComponent<T>(anEntity);
}

