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

	myDynamicsWorld->setGravity(btVector3(0, -1, 0));


	/* Create a new class for this to make it easier to work with. */
	/*myZeroShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	myZeroMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo bodyInfo(0, myZeroMotionState, myZeroShape, btVector3(0, 0, 0));
	myZeroPlane = new btRigidBody(bodyInfo);*/

	myZeroPlane = new CRigidBody();
	Add(myZeroPlane->InitAsPlane(btVector3(0, 1, 0)));
}


CPhysicsManager::~CPhysicsManager()
{
	/* Remove all the rigidbodies before deleting */
	myDynamicsWorld->removeRigidBody(myZeroPlane->GetBody());

	SAFE_DELETE(myDefaultCollisionConfiguration);
	SAFE_DELETE(myCollisionDispatcher);
	SAFE_DELETE(myBroadphaseInterface);
	SAFE_DELETE(myDynamicsWorld);
}

void CPhysicsManager::Add(btRigidBody* aBody)
{
	myDynamicsWorld->addRigidBody(aBody);
}

btRigidBody* CPhysicsManager::Create(float mass)
{
	return nullptr;
}

void CPhysicsManager::Update()
{
	myDynamicsWorld->stepSimulation(1.f / 60.f, 1);
}
