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
	double myAccumulatedTime = 0.f;
	Snowblind::CTerrain* myTerrain = nullptr;
	Snowblind::CSynchronizer* mySynchronizer;
	Snowblind::CEngine* myEngine;
	CEntityManager* myEntityManager;
	CPhysicsManager* myPhysicsManager;
};

