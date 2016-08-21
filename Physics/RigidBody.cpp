#include "stdafx.h"
#include "RigidBody.h"
#include "PhysicsDefines.h"
#include <BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h>
#include <Utilities.h>

CRigidBody::CRigidBody()
{
}

CRigidBody::~CRigidBody()
{
	SAFE_DELETE(myVertexArray);
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

btRigidBody* CRigidBody::InitAsTerrain(CU::GrowingArray<float> vertices, CU::GrowingArray<s32> indices)
{
	s32 faceCount = indices.Size() / 3;
	s32 vStride = sizeof(CU::Vector3f);
	s32 iStride = sizeof(u32) * 3;

	btScalar* locVertices = new btScalar[vertices.Size()];
	memcpy(&locVertices[0], &vertices[0], sizeof(float) * vertices.Size());
	s32* locIndices = new s32[indices.Size()];
	memcpy(&locIndices[0], &indices[0], sizeof(s32) * indices.Size());

	myVertexArray = new btTriangleIndexVertexArray(faceCount, locIndices, iStride, vertices.Size(), locVertices, vStride);
	myShape = new btBvhTriangleMeshShape(myVertexArray, false, btVector3(0, 0, 0), btVector3(1, 1, 1), true);
	myMotionState = new btDefaultMotionState();
	myWorldTranslation = &myMotionState->m_graphicsWorldTrans;

	btRigidBody::btRigidBodyConstructionInfo bodyInfo(0, myMotionState, myShape, btVector3(0, 0, 0));
	myBody = new btRigidBody(bodyInfo);
	return myBody;
}

btRigidBody* CRigidBody::InitAsSphere(float aRadius, float aMass, float aGravityForce, float anInitialResistance, const CU::Vector3f& aPosition)
{
	myRadius = aRadius;
	myGravity = aGravityForce;
	SetResistanceDensity(anInitialResistance);
	myMass = aMass;
	myDragCoeff = 0.47f;
	myCrossSectionArea = (myRadius * myRadius) * 3.14f;

	btVector3 pos = btVector3(aPosition.x, aPosition.y, aPosition.z); //initial position
	myShape = new btSphereShape(myRadius);
	myMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos)); /* btQuaternion is the rotation of the object. Figure this out.*/
	btRigidBody::btRigidBodyConstructionInfo bodyInfo(myMass, myMotionState, myShape, btVector3(0, 0, 0));
	//bodyInfo.m_friction = 1.f;
	//bodyInfo.m_restitution = 0.75f;

	myBody = new btRigidBody(bodyInfo);
	myBody->setMassProps(myMass, btVector3(0, 0, 0));
	myWorldTranslation = &myMotionState->m_graphicsWorldTrans;
	myTerminalVelocity.y = CL::CalcTerminalVelocity(myMass, myGravity, myDragCoeff, myCrossSectionArea, myResistanceDensity);
	return myBody;
}

void CRigidBody::SetResistanceDensity(float aDensity)
{
	myResistanceDensity = aDensity;
}

void CRigidBody::SetPosition(const CU::Vector3f& aPosition)
{
	assert("Not Implemented");
}

void CRigidBody::Update(float deltaTime)
{
	if (myVelocity.y < myTerminalVelocity.y)
	{
		float a = CL::CalcAcceleration(myGravity, myMass);
		myVelocity.y += a * deltaTime;
	}
	
	if(myVelocity.y > myTerminalVelocity.y)
		myVelocity.y = myTerminalVelocity.y;

	float drag = CL::CalcDrag(myResistanceDensity, myVelocity.y, myDragCoeff, myCrossSectionArea);
	float temp = myVelocity.y - drag;
	myBody->setLinearVelocity(btVector3(0, temp, 0));
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

