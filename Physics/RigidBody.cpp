#include "stdafx.h"
#include "RigidBody.h"
#include "PhysicsDefines.h"

#ifdef _DEBUG
#include <RenderCommand.h>
#include <Synchronizer.h>
#endif


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
	bodyInfo.m_friction = 1.5f;
	bodyInfo.m_restitution = 1.0f;

	myBody = new btRigidBody(bodyInfo);

	myWorldTranslation = &myMotionState->m_graphicsWorldTrans;

	return myBody;
}

btRigidBody* CRigidBody::InitAsSphere(const CU::Vector3f& aPosition)
{
	btVector3 pos;
	pos.setX(aPosition.x);
	pos.setY(aPosition.y);
	pos.setZ(aPosition.z);
	myShape = new btSphereShape(1); 
	myMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos)); /* btQuaternion is the rotation of the object. Figure this out.*/
	btRigidBody::btRigidBodyConstructionInfo bodyInfo(100, myMotionState, myShape, btVector3(0, 0, 0));
	//bodyInfo.m_friction = 1.f;
	//bodyInfo.m_restitution = 0.75f;


	myBody = new btRigidBody(bodyInfo);
	myBody->setMassProps(100, btVector3(0, 0, 0));
	myWorldTranslation = &myMotionState->m_graphicsWorldTrans;
	
	return myBody;
}

btRigidBody* CRigidBody::GetBody()
{
	return myBody;
}

const CU::Matrix44f& CRigidBody::GetOrientation()
{
	myWorldTranslation->getOpenGLMatrix(&myOrientation.myMatrix[0]);
	return myOrientation;
}

void CRigidBody::Impulse(const CU::Vector3f& anImpulseVector)
{
	myBody->applyCentralImpulse(btVector3(anImpulseVector.x, anImpulseVector.y, anImpulseVector.z));
}

