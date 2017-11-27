#include "stdafx.h"
#include "PhysicsManager.h"
#include "PhysicsDefines.h"
#include "../CommonLib/Math/Matrix/Matrix.h"
#include "RigidBody.h"
#include <Engine/Engine.h>
//Had to be included to add to phys world?
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

PhysicsManager::PhysicsManager()
	: m_Gravity(9.82f)
{
	myDefaultCollisionConfiguration = new btDefaultCollisionConfiguration();
	myCollisionDispatcher = new btCollisionDispatcher(myDefaultCollisionConfiguration);
	myBroadphaseInterface = new btDbvtBroadphase();
	myImpulseSolver = new btSequentialImpulseConstraintSolver();


	myDynamicsWorld = new btDiscreteDynamicsWorld(
		myCollisionDispatcher,
		myBroadphaseInterface,
		myImpulseSolver,
		myDefaultCollisionConfiguration);

	myDynamicsWorld->setGravity(btVector3(0, -m_Gravity, 0));
	myZeroPlane = new RigidBody();
	Add(myZeroPlane->InitAsPlane(btVector3(0, 1, 0)));
}


PhysicsManager::~PhysicsManager()
{
	/* Remove all the rigidbodies before deleting */
	for (btCollisionObject* rb : myBodies)
	{
		Remove(rb);
	}
	Remove(myZeroPlane->GetBody());

	SAFE_DELETE(myDefaultCollisionConfiguration);
	SAFE_DELETE(myCollisionDispatcher);
	SAFE_DELETE(myBroadphaseInterface);
	SAFE_DELETE(myDynamicsWorld);
	SAFE_DELETE(myImpulseSolver);
}

void PhysicsManager::Add(btRigidBody* aBody)
{
	BeginTicketMutex(&m_Mutex);
	myDynamicsWorld->addRigidBody(aBody);
	myBodies.Add(aBody);
	EndTicketMutex(&m_Mutex);
}

void PhysicsManager::Add(btCollisionObject* collision_object)
{
	BeginTicketMutex(&m_Mutex);
	myDynamicsWorld->addCollisionObject(collision_object);
	myBodies.Add(collision_object);
	EndTicketMutex(&m_Mutex);
}

void PhysicsManager::Add(btGhostObject* ghost_object)
{
	BeginTicketMutex(&m_Mutex);
	myDynamicsWorld->addCollisionObject(ghost_object);
	myBodies.Add(ghost_object);
	EndTicketMutex(&m_Mutex);
}

void PhysicsManager::Remove(btRigidBody* aBody)
{
	myDynamicsWorld->removeRigidBody(aBody);
}

void PhysicsManager::Remove(btCollisionObject* collision_object)
{
	myDynamicsWorld->removeCollisionObject(collision_object);
}

RigidBody* PhysicsManager::CreateBody()
{
	RigidBody* newBody = new RigidBody();
	return newBody;
}

float PhysicsManager::GetGravityForce()
{
	return m_Gravity;
}

void PhysicsManager::Update()
{
	float frame_time = Engine::GetInstance()->GetDeltaTime();
	while (frame_time >= (1.f / 60.f))
	{
		myDynamicsWorld->stepSimulation(1.f / 60.f, 8);
		frame_time -= (1.f / 60.f);
	}
}

CU::Vector3f PhysicsManager::RayCast(const CU::Vector3f& cameraPosition, const CU::Vector3f& target) const
{
	const float vector_extent = 125.f;

	
	btVector3 from = btu::ConvertVector(cameraPosition);
	btVector3 to = btu::ConvertVector(cameraPosition + (target * vector_extent));

	btCollisionWorld::ClosestRayResultCallback result(from, to);
	myDynamicsWorld->rayTest(from, to, result);
	if (result.hasHit())
		return btu::ConvertVector(result.m_hitPointWorld);

	return CU::Vector3f();
}
