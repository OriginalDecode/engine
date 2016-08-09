#pragma once

namespace Snowblind
{
	class CSynchronizer;
	class CTerrain;
	class CEngine;
};

class CEntityManager;
class CPhysicsManager;
class btRigidBody;

class CGame
{
public:
	CGame(Snowblind::CSynchronizer* aSynchronizer);
	~CGame();

	void Update(float aDeltaTime);

private:
	double myAccumulatedTime = 0.0;
	
	int myFrameCount = 0;
	float myAverageFPS = 0;
	float myTime = 0.f;
	int myFPSToPrint = 0;

	Snowblind::CTerrain* myTerrain = nullptr;
	Snowblind::CSynchronizer* mySynchronizer;
	Snowblind::CEngine* myEngine;
	CEntityManager* myEntityManager;
	CPhysicsManager* myPhysicsManager;
};

