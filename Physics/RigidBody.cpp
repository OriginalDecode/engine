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

#ifdef _DEBUG
CRigidBody::CRigidBody(Snowblind::CSynchronizer* aSychronizer)
	: mySynchronizer(aSychronizer)
{
}
#endif

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


	
	//myBody->getAabb()


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
	myShape->calculateLocalInertia(1, btVector3(0, 0, 0));

	btRigidBody::btRigidBodyConstructionInfo bodyInfo(10, myMotionState, myShape, btVector3(0, 0, 0));
	bodyInfo.m_friction = 1.f;
	bodyInfo.m_restitution = 0.75f;
	myBody = new btRigidBody(bodyInfo);
	myWorldTranslation = &myMotionState->m_graphicsWorldTrans;




	return myBody;
}

btRigidBody* CRigidBody::GetBody()
{
	return myBody;
}

const CU::Matrix44f& CRigidBody::GetOrientation()
{
#ifdef _DEBUG
	CalculatePoints();

	SLinePoint p1;
	SLinePoint p2;
	p1.position = myPoints[0];
	p1.color = CU::Vector4f(1.f, 0.f, 0.f, 1.f);
	p2.position = myPoints[1];
	p2.color = CU::Vector4f(1.f, 1.f, 0.f, 1.f);
	mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::LINE, p1, p2));

	p1.position = myPoints[1];
	p1.color = CU::Vector4f(1.f, 1.f, 0.f, 1.f);
	p2.position = myPoints[2];
	p2.color = CU::Vector4f(1.f, 1.f, 1.f, 1.f);
	mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::LINE, p1, p2));

	p1.position = myPoints[2];
	p1.color = CU::Vector4f(1.f, 1.f, 1.f, 1.f);
	p2.position = myPoints[3];
	p2.color = CU::Vector4f(1.f, 0.f, 1.f, 1.f);
	mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::LINE, p1, p2));

	p1.position = myPoints[3];
	p1.color = CU::Vector4f(1.f, 0.f, 1.f, 1.f);
	p2.position = myPoints[4];
	p2.color = CU::Vector4f(0.f, 0.f, 1.f, 1.f);
	mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::LINE, p1, p2));

	p1.position = myPoints[4];
	p1.color = CU::Vector4f(0.f, 0.f, 1.f, 1.f);
	p2.position = myPoints[5];
	p2.color = CU::Vector4f(0.f, 1.f, 0.f, 1.f);
	mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::LINE, p1, p2));

	p1.position = myPoints[5];
	p1.color = CU::Vector4f(0.f, 1.f, 0.f, 1.f);
	p2.position = myPoints[6];
	p2.color = CU::Vector4f(0.f, 0.f, 0.f, 1.f);
	mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::LINE, p1, p2));

	p1.position = myPoints[6];
	p1.color = CU::Vector4f(0.f, 0.f, 0.f, 1.f);
	p2.position = myPoints[7];
	p2.color = CU::Vector4f(1.f, 1.f, 1.f, 1.f);
	mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::LINE, p1, p2));
	//
	p1.position = myPoints[7];
	p1.color = CU::Vector4f(1.f, 1.f, 1.f, 1.f);
	p2.position = myPoints[4];
	p2.color = CU::Vector4f(0.f, 0.f, 1.f, 1.f);
	mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::LINE, p1, p2));

	p1.position = myPoints[7];
	p1.color = CU::Vector4f(0.f, 0.f, 0.f, 1.f);
	p2.position = myPoints[2];
	p2.color = CU::Vector4f(1.f, 1.f, 1.f, 1.f);
	mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::LINE, p1, p2));

	p1.position = myPoints[6];
	p1.color = CU::Vector4f(0.f, 0.f, 0.f, 1.f);
	p2.position = myPoints[1];
	p2.color = CU::Vector4f(1.f, 1.f, 0.f, 1.f);
	mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::LINE, p1, p2));

	p1.position = myPoints[0];
	p1.color = CU::Vector4f(1.f, 0.f, 0.f, 1.f);
	p2.position = myPoints[5];
	p2.color = CU::Vector4f(0.f, 1.f, 0.f, 1.f);
	mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::LINE, p1, p2));

	p1.position = myPoints[0];
	p1.color = CU::Vector4f(1.f, 0.f, 0.f, 1.f);
	p2.position = myPoints[3];
	p2.color = CU::Vector4f(1.f, 0.f, 1.f, 1.f);
	mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::LINE, p1, p2));
#endif

	myWorldTranslation->getOpenGLMatrix(&myOrientation.myMatrix[0]);
	return myOrientation;
}

const CU::Vector4f& CRigidBody::GetAABBPoint(int index)
{
	return myPoints[index];
}

void CRigidBody::CalculatePoints()
{
	btVector3 min;
	btVector3 max;
	myBody->getAabb(min, max);

	float depth = max.getZ() - min.getZ();
	float width = max.getX() - min.getX();
	float height = max.getY() - min.getY();

	myPoints[0].x = min.getX();
	myPoints[0].y = min.getY();
	myPoints[0].z = min.getZ();

	
	myPoints[1] = myPoints[0];
	myPoints[1].x += width;

	myPoints[2] = myPoints[1];
	myPoints[2].z += depth;
	
	myPoints[3] = myPoints[2];
	myPoints[3].x -= width;

	myPoints[4] = myPoints[3];
	myPoints[4].y += height;

	myPoints[5] = myPoints[4];
	myPoints[5].z -= depth;

	myPoints[6] = myPoints[5];
	myPoints[6].x += width;
	
	myPoints[7] = myPoints[6];
	myPoints[7].z += depth;

}
