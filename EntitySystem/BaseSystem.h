#pragma once
#include "ComponentFilter.h"
#include "NodeEntityManager.h"
#include "EntityManager.h"
#include <Utilities.h>

class Synchronizer;
class Engine;
class BaseSystem
{
public:
	BaseSystem(NodeEntityManager& anEntityManager);
	BaseSystem(NodeEntityManager& anEntityManager, const ComponentFilter& aFilter);
	virtual ~BaseSystem() = 0;

	virtual void Update(float aDeltaTime) = 0;
	const CU::GrowingArray<Entity>& GetEntities();

	template<typename T>
	T& GetComponent(Entity anEntity);

private:
	void operator=(BaseSystem&) = delete;

protected:
	ComponentFilter myFilter;
	NodeEntityManager& m_Manager;
	Engine* m_Engine = nullptr;
	
};

template<typename T>
T& BaseSystem::GetComponent(Entity anEntity)
{
	return m_Manager.GetComponent<T>(anEntity);
}

