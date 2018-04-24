#pragma once
#include "ComponentFilter.h"
#include "NodeEntityManager.h"
#include "EntityManager.h"
#include <CommonLib/Utilities.h>

class Synchronizer;
class Engine;
#define _PER_NODE_SYSTEM
#ifdef _PER_NODE_SYSTEM
typedef NodeEntityManager _EntityManager;
#else
typedef EntityManager _EntityManager;
#endif

class BaseSystem
{
public:
	BaseSystem(_EntityManager& anEntityManager);
	BaseSystem(_EntityManager& anEntityManager, const ComponentFilter& aFilter);
	virtual ~BaseSystem() = 0;

	virtual void Update(float dt, bool paused) = 0;
	const CU::GrowingArray<Entity>& GetEntities();

	template<typename T>
	T& GetComponent(Entity anEntity);

private:
	void operator=(BaseSystem&) = delete;

protected:
	ComponentFilter myFilter;
	_EntityManager& m_Manager;
	Engine* m_Engine = nullptr;
	
};

template<typename T>
T& BaseSystem::GetComponent(Entity anEntity)
{
	return m_Manager.GetComponent<T>(anEntity);
}

