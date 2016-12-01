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
	class Synchronizer;
	class CTerrain;
	class Engine;
	class CMousePicker;
	class Camera;
};
class JSONReader;
class CEntityManager;
class CPhysicsManager;
class RigidBody;
struct InputController;

static void Jump(RigidBody* rigidbody);
static void Forward(RigidBody* rigidbody);
static void Backward(RigidBody* rigidbody);
static void Right(RigidBody* rigidbody);
static void Left(RigidBody* rigidbody);

static void Up(Snowblind::Camera* camera);
static void Forward(Snowblind::Camera* camera);
static void Backward(Snowblind::Camera* camera);
static void Right(Snowblind::Camera* camera);
static void Left(Snowblind::Camera* camera);
static void Down(Snowblind::Camera* camera);


class Game
{
public:
	Game() = default;
	bool Initiate(Snowblind::Synchronizer* synchronizer);
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
	CU::GrowingArray<RigidBody*> myTerrainBodies;

	Snowblind::Synchronizer* mySynchronizer = nullptr;
	Snowblind::Engine* myEngine = nullptr;
	Snowblind::CMousePicker* myPicker = nullptr;

	CEntityManager* myEntityManager = nullptr;
	CPhysicsManager* myPhysicsManager = nullptr;

	std::string m_ModelKey;

	CU::Vector3f pointHit;
	CU::Vector3f currentRay;

	u32 selected_entity;


	RigidBody* rigidbody = nullptr;
	SLinePoint raycast[2];
};


