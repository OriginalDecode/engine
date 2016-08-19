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
	
	myZeroPlane = new CRigidBody(0);
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

CRigidBody* CPhysicsManager::CreateBody(float mass)
{
	CRigidBody* newBody = new CRigidBody(mass);
	return newBody;
}

void CPhysicsManager::Update(double& additionalTime)
{
		myDynamicsWorld->stepSimulation(1.f / 60.f, 1);
}
