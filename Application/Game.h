#pragma once
#include <DataStructures/GrowingArray.h>
#include <Math/Vector/Vector.h>
#include <string>
#include <Line3D.h>
#include <JSON/JSONReader.h>
#include <standard_datatype.hpp>
#include <functional>
#include <AABB.h>
#include <Math/Plane/Plane.h>
#include <Math/Plane/PlaneVolume.h>
#include <Subscriber.h>
namespace Hex
{
	class Synchronizer;
	class CTerrain;
	class Engine;
	class CMousePicker;
	class Camera;
};
class JSONReader;
class RigidBody;
struct InputComponent;

class Game : public Subscriber
{
public:
	Game() = default;
	bool Initiate();
	bool CleanUp();

	void Update(float aDeltaTime);

private:
	bool m_DisableMouseCameraMovement = false;
	Hex::Camera* m_Camera = nullptr;


	bool m_collision = false;

	s16 m_LocalPlayerCount = 0;
	int myFrameCount = 0;
	int myFPSToPrint = 0;
	float myAverageFPS = 0;
	float myTime = 0.f;

	CU::GrowingArray<cl::AABB> m_AABBs;
	CU::GrowingArray<Hex::CTerrain*> myTerrain;
	CU::GrowingArray<RigidBody*> myTerrainBodies;

	Hex::Synchronizer* mySynchronizer = nullptr;
	Hex::Engine* myEngine = nullptr;
	Hex::CMousePicker* myPicker = nullptr;


	std::string m_ModelKey;

	CU::Vector3f pointHit;
	CU::Vector3f currentRay;

	u32 selected_entity;

	CU::PlaneVolume<float> m_PlaneVolume;

	RigidBody* rigidbody = nullptr;
	SLinePoint raycast[2];

	void ReceiveMessage(const OnLeftClick& message);
	

};


