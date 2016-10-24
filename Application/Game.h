#pragma once
#include <DataStructures/GrowingArray.h>
#include <Math/Vector/Vector.h>
#include <string>
#include <Line3D.h>
#include <JSON/JSONReader.h>

namespace Snowblind
{
	class CSynchronizer;
	class CTerrain;
	class CEngine;
	class CMousePicker;
};
class JSONReader;
class CEntityManager;
class CPhysicsManager;
class CRigidBody;

class CGame
{
public:
	CGame() = default;
	bool Initiate(Snowblind::CSynchronizer* synchronizer);
	bool CleanUp();

	void Update(float aDeltaTime);

private:

	bool CreateLevel(const char* level_path);
	bool CreateEntity(const char* entity_path, JSONReader& level_reader, JSONElement::ConstMemberIterator it);


	int myFrameCount = 0;
	int myFPSToPrint = 0;
	float myAverageFPS = 0;
	float myTime = 0.f;

	CU::GrowingArray<Snowblind::CTerrain*> myTerrain;
	CU::GrowingArray<CRigidBody*> myTerrainBodies;

	Snowblind::CSynchronizer* mySynchronizer = nullptr;
	Snowblind::CEngine* myEngine = nullptr;
	Snowblind::CMousePicker* myPicker = nullptr;

	CEntityManager* myEntityManager = nullptr;
	CPhysicsManager* myPhysicsManager = nullptr;

	std::string m_ModelKey;

	CU::Vector3f pointHit;
	CU::Vector3f currentRay;

	CRigidBody* rigidbody;
	SLinePoint raycast[2];
};

