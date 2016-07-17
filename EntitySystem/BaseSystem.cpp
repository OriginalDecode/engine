#include "BaseSystem.h"



CBaseSystem::CBaseSystem(CEntityManager& anEntityManager)
	: myEntityManager(anEntityManager)
{
}


CBaseSystem::CBaseSystem(CEntityManager& anEntityManager, const SComponentFilter& aFilter)
	: myEntityManager(anEntityManager)
	, myFilter(aFilter)
{
}

CBaseSystem::~CBaseSystem()
{
}

bool CBaseSystem::HasFinished()
{
	return myHasFinished;
}

const CU::GrowingArray<Entity>& CBaseSystem::GetEntities()
{
	return myEntityManager.GetEntities(myFilter);
}

