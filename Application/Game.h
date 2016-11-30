#pragma once
#include <DataStructures/GrowingArray.h>
#include <Math/Vector/Vector.h>
#include <string>
#include <Line3D.h>
#include <JSON/JSONReader.h>
#include <standard_datatype.hpp>
#include <functional>
#include <AABB.h>

namespace Snowblind
{
	class CSynchronizer;
	class CTerrain;
	class Engine;
	class CMousePicker;
	class Camera;
};
class JSONReader;
class CEntityManager;
class CPhysicsManager;
class CRigidBody;
struct InputController;

static void Jump(CRigidBody* rigidbody);
static void Forward(CRigidBody* rigidbody);
static void Backward(CRigidBody* rigidbody);
static void Right(CRigidBody* rigidbody);
static void Left(CRigidBody* rigidbody);

static void Up(Snowblind::Camera* camera);
static void Forward(Snowblind::Camera* camera);
static void Backward(Snowblind::Camera* camera);
static void Right(Snowblind::Camera* camera);
static void Left(Snowblind::Camera* camera);
static void Down(Snowblind::Camera* camera);


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

	void LeftClick(float x, float y);
	bool m_DisableMouseCameraMovement = false;



	s16 m_LocalPlayerCount = 0;
	int myFrameCount = 0;
	int myFPSToPrint = 0;
	float myAverageFPS = 0;
	float myTime = 0.f;

	CU::GrowingArray<cl::AABB> m_AABBs;
	CU::GrowingArray<Snowblind::CTerrain*> myTerrain;
	CU::GrowingArray<CRigidBody*> myTerrainBodies;

	Snowblind::CSynchronizer* mySynchronizer = nullptr;
	Snowblind::Engine* myEngine = nullptr;
	Snowblind::CMousePicker* myPicker = nullptr;

	CEntityManager* myEntityManager = nullptr;
	CPhysicsManager* myPhysicsManager = nullptr;

	std::string m_ModelKey;

	CU::Vector3f pointHit;
	CU::Vector3f currentRay;

	u32 selected_entity;


	CRigidBody* rigidbody = nullptr;
	SLinePoint raycast[2];
};


