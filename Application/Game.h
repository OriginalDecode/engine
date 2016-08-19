#pragma once
#include <DataStructures/GrowingArray.h>

namespace Snowblind
{
	class CSynchronizer;
	class CTerrain;
	class CEngine;
};

class CEntityManager;
class CPhysicsManager;
class CRigidBody;

class CGame
{
public:
	CGame(Snowblind::CSynchronizer* aSynchronizer);
	~CGame();

	void Update(float aDeltaTime);

private:
	
	int myFrameCount = 0;
	float myAverageFPS = 0;
	float myTime = 0.f;
	int myFPSToPrint = 0;

	CU::GrowingArray<Snowblind::CTerrain*> myTerrain;
	CU::GrowingArray<CRigidBody*> myTerrainBodies;

	Snowblind::CSynchronizer* mySynchronizer;
	Snowblind::CEngine* myEngine;
	CEntityManager* myEntityManager;
	CPhysicsManager* myPhysicsManager;
};

