#pragma once
#include "BaseSystem.h"
class CEntityManager;
class CPhysicsSystem : public CBaseSystem 
{
public:
	CPhysicsSystem(CEntityManager& anEntityManager);
	~CPhysicsSystem();

	void Update(float aDeltaTime) override;

private:

};

