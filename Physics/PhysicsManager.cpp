#include "stdafx.h"
#include "PhysicsManager.h"
#include "PhysicsDefines.h"
#include "../CommonLib/Math/Matrix/Matrix.h"
#include "RigidBody.h"
#include <Windows.h>
CPhysicsManager::CPhysicsManager()
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

	myDynamicsWorld->setGravity(btVector3(0, -9.82f, 0));

	myZeroPlane = new CRigidBody();
	Add(myZeroPlane->InitAsPlane(btVector3(0, 1, 0)));
}


CPhysicsManager::~CPhysicsManager()
{
	/* Remove all the rigidbodies before deleting */
	for each(btRigidBody* rb in myBodies)
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

btRigidBody* CPhysicsManager::Create(float mass)
{
	return nullptr;
}

void CPhysicsManager::Update()
{
	myDynamicsWorld->stepSimulation(1.f/60.f, 1);
}
