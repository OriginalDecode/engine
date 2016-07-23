#include "stdafx.h"
#include "RigidBody.h"
#include "PhysicsDefines.h"

CRigidBody::CRigidBody()
{
}


CRigidBody::~CRigidBody()
{
	SAFE_DELETE(myShape);
	SAFE_DELETE(myMotionState);
	SAFE_DELETE(myBody);
}

btRigidBody* CRigidBody::InitAsPlane(const btVector3& aNormal)
{
	myShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	myMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo bodyInfo(0, myMotionState, myShape, btVector3(0, 0, 0));
	myBody = new btRigidBody(bodyInfo);

	return myBody;
}

btRigidBody* CRigidBody::InitAsSphere()
{
	myShape = new btSphereShape(1);
	myMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
	myShape->calculateLocalInertia(1, btVector3(0, 0, 0));

	btRigidBody::btRigidBodyConstructionInfo bodyInfo(1, myMotionState, myShape, btVector3(0, 0, 0));
	myBody = new btRigidBody(bodyInfo);
	/* Make this work somehow. Relate the worldTrans matrix to the orienttation matirx. */
	myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(&myOrientation->myMatrix[0]);
	return myBody;
}

btRigidBody* CRigidBody::GetBody()
{
	return myBody;
}

const CU::Matrix44f& CRigidBody::GetPosition()
{
	return *myOrientation;
}
