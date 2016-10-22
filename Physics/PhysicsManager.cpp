#include "stdafx.h"
#include "PhysicsManager.h"
#include "PhysicsDefines.h"
#include "../CommonLib/Math/Matrix/Matrix.h"
#include "RigidBody.h"
#include <Windows.h>
CPhysicsManager::CPhysicsManager()
	: myGravity(9.82f)
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

	myDynamicsWorld->setGravity(btVector3(0, -myGravity, 0));
	myZeroPlane = new CRigidBody();
	Add(myZeroPlane->InitAsPlane(btVector3(0, 1, 0)));
}


CPhysicsManager::~CPhysicsManager()
{
	/* Remove all the rigidbodies before deleting */
	for (btRigidBody* rb : myBodies)
	{
		Remove(rb);
	}
	Remove(myZeroPlane->GetBody());

	SAFE_DELETE(myDefaultCollisionConfiguration);
	SAFE_DELETE(myCollisionDispatcher);
	SAFE_DELETE(myBroadphaseInterface);
	SAFE_DELETE(myDynamicsWorld);
}

void CPhysicsManager::Add(btRigidBody* aBody)
{
	myDynamicsWorld->addRigidBody(aBody);
	myBodies.Add(aBody);
}

void CPhysicsManager::Remove(btRigidBody* aBody)
{
	myDynamicsWorld->removeRigidBody(aBody);
}

CRigidBody* CPhysicsManager::CreateBody()
{
	CRigidBody* newBody = new CRigidBody();
	return newBody;
}

float CPhysicsManager::GetGravityForce()
{
	return myGravity;
}

void CPhysicsManager::Update(double& additionalTime)
{
	myDynamicsWorld->stepSimulation(1.f / 60.f, 8);
}

CU::Vector3f CPhysicsManager::RayCast(const CU::Vector3f& cameraPosition, const CU::Vector3f& target) const
{
	const float vector_extent = 125.f;
	btVector3 from = btVector3(cameraPosition.x, cameraPosition.y, cameraPosition.z);
	btVector3 to = btVector3(cameraPosition.x + (target.x * vector_extent), cameraPosition.y + (target.y * vector_extent), cameraPosition.z + (target.z * vector_extent));

	btCollisionWorld::ClosestRayResultCallback result(from, to);
	myDynamicsWorld->rayTest(from, to, result);
	if (result.hasHit())
		return CU::Vector3f(result.m_hitPointWorld.getX(), result.m_hitPointWorld.getY(), result.m_hitPointWorld.getZ());


	return CU::Vector3f();
}
