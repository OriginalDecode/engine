#include "stdafx.h"
#include "RigidBody.h"
#include "PhysicsDefines.h"
#include <BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h>

#include <Utilities.h>

RigidBody::RigidBody()
{
}

RigidBody::~RigidBody()
{
	SAFE_DELETE(myVertexArray);
	SAFE_DELETE(m_Shape);
	SAFE_DELETE(m_MotionState);
	SAFE_DELETE(myBody);

}

btRigidBody* RigidBody::InitAsPlane(const btVector3& aNormal)
{
	m_Shape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	m_MotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo bodyInfo(0, m_MotionState, m_Shape, btVector3(0, 0, 0));
	bodyInfo.m_friction = 1.5f;
	bodyInfo.m_restitution = 1.0f;

	myBody = new btRigidBody(bodyInfo);

	myWorldTranslation = &m_MotionState->m_graphicsWorldTrans;

	return myBody;
}

btRigidBody* RigidBody::InitAsTerrain(std::vector<float> vertices, std::vector<s32> indices)
{
	s32 faceCount = (s32)indices.size() / 3;
	s32 vStride = sizeof(CU::Vector3f);
	s32 iStride = sizeof(u32) * 3;

	btScalar* locVertices = new btScalar[vertices.size()];
	memcpy(&locVertices[0], &vertices[0], sizeof(float) * vertices.size());
	s32* locIndices = new s32[indices.size()];
	memcpy(&locIndices[0], &indices[0], sizeof(s32) * indices.size());

	myVertexArray = new btTriangleIndexVertexArray(faceCount, locIndices, iStride, (s32)vertices.size(), locVertices, vStride);
	m_Shape = new btBvhTriangleMeshShape(myVertexArray, false, btVector3(0, 0, 0), btVector3(1, 1, 1), true);
	m_MotionState = new btDefaultMotionState();
	myWorldTranslation = &m_MotionState->m_graphicsWorldTrans;

	btRigidBody::btRigidBodyConstructionInfo bodyInfo(0, m_MotionState, m_Shape, btVector3(0, 0, 0));
	myBody = new btRigidBody(bodyInfo);
	return myBody;
}

btRigidBody* RigidBody::InitAsSphere(float aRadius, float aMass, float aGravityForce, float anInitialResistance, const CU::Vector3f& aPosition)
{
	myRadius = aRadius;
	myGravity = aGravityForce;
	SetResistanceDensity(anInitialResistance);
	myMass = aMass;
	myDragCoeff = 0.47f;
	myCrossSectionArea = (myRadius * myRadius) * 3.14f;

	btVector3 pos = btVector3(aPosition.x, aPosition.y, aPosition.z); //initial position
	m_Shape = new btSphereShape(myRadius);
	m_MotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos)); /* btQuaternion is the rotation of the object. Figure this out.*/
	btRigidBody::btRigidBodyConstructionInfo bodyInfo(myMass, m_MotionState, m_Shape, btVector3(0, 0, 0));
	
	//bodyInfo.m_friction = 1.f;
	//bodyInfo.m_restitution = 0.75f;

	myBody = new btRigidBody(bodyInfo);
	myBody->setActivationState(DISABLE_DEACTIVATION);
	myBody->setMassProps(myMass, btVector3(0, 0, 0));
	myWorldTranslation = &m_MotionState->m_graphicsWorldTrans;
	myTerminalVelocity.y = CL::CalcTerminalVelocity(myMass, myGravity, myDragCoeff, myCrossSectionArea, myResistanceDensity);
	
	return myBody;
}

btRigidBody* RigidBody::InitWithMeshCollision(std::vector<float> vertices, std::vector<s32> indices)
{
	if (vertices.size() <= 0 || indices.size() <= 0)
		return nullptr;

	s32 faceCount = (s32)indices.size() / 3;
	s32 vStride = sizeof(CU::Vector3f);
	s32 iStride = sizeof(u32) * 3;

	btScalar* locVertices = new btScalar[vertices.size()];
	memcpy(&locVertices[0], &vertices[0], sizeof(float) * vertices.size());
	s32* locIndices = new s32[indices.size()];
	memcpy(&locIndices[0], &indices[0], sizeof(s32) * indices.size());

	myVertexArray = new btTriangleIndexVertexArray(faceCount, locIndices, iStride, (s32)vertices.size(), locVertices, vStride);
	m_Shape = new btBvhTriangleMeshShape(myVertexArray, false, btVector3(0, 0, 0), btVector3(1, 1, 1), true);
	m_MotionState = new btDefaultMotionState();
	myWorldTranslation = &m_MotionState->m_graphicsWorldTrans;

	btRigidBody::btRigidBodyConstructionInfo bodyInfo(0, m_MotionState, m_Shape, btVector3(0, 0, 0));
	myBody = new btRigidBody(bodyInfo);
	return myBody;
}

btRigidBody* RigidBody::InitAsBox(float width, float height, float depth, CU::Vector3f position)
{
	myMass = 0.f;
	m_Shape = new btBoxShape(btVector3(width, height, depth));
	btVector3 pos = btVector3(position.x, position.y, position.z); //initial position
	m_MotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
	btRigidBody::btRigidBodyConstructionInfo bodyInfo(0, m_MotionState, m_Shape, btVector3(0, 0, 0));
	myBody = new btRigidBody(bodyInfo);
	myWorldTranslation = &m_MotionState->m_graphicsWorldTrans;

	return myBody;
}

void RigidBody::SetResistanceDensity(float aDensity)
{
	myResistanceDensity = aDensity;
}

void RigidBody::SetPosition(const CU::Vector3f& aPosition)
{
	myBody->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), bullet_utilities::ConvertVector(aPosition) ));
}

void RigidBody::Update(float deltaTime)
{
	// (#LINUS) Terrain should probably be above 0y so that we can make a arbitrary check to see if you are out of bounds
	btVector3 linear_velocity = myBody->getLinearVelocity();
	myVelocity.x = linear_velocity.getX();
	myVelocity.y = linear_velocity.getY();
	myVelocity.z = linear_velocity.getZ();

	if (myVelocity.x > 50.f)
		myVelocity.x = 50.f;
	if (myVelocity.x < -50.f)
		myVelocity.x = -50.f;


	if (myVelocity.y > 50.f)
		myVelocity.y = 50.f;
	if (myVelocity.y < -50.f)
		myVelocity.y = -50.f;

	if (myVelocity.z > 50.f)
		myVelocity.z = 50.f;
	if (myVelocity.z < -50.f)
		myVelocity.z = -50.f;

	myBody->setLinearVelocity(btVector3(myVelocity.x, myVelocity.y, myVelocity.z));
}

btRigidBody* RigidBody::GetBody()
{
	return myBody;
}

const CU::Matrix44f& RigidBody::GetOrientation()
{
	myWorldTranslation->getOpenGLMatrix(&m_Orientation.myMatrix[0]);

	CU::Vector3f axisX(1.f, 0.f, 0.f);
	CU::Vector3f axisY(0.f, 1.f, 0.f);
	CU::Vector3f axisZ(0.f, 0.f, 1.f);

	axisX = m_Yaw * m_Pitch * axisX;
	axisY = m_Yaw * m_Pitch * axisY;
	axisZ = m_Yaw * m_Pitch * axisZ;

	m_Orientation[0] = axisX.x;
	m_Orientation[1] = axisX.y;
	m_Orientation[2] = axisX.z;
	m_Orientation[4] = axisY.x;
	m_Orientation[5] = axisY.y;
	m_Orientation[6] = axisY.z;
	m_Orientation[8] = axisZ.x;
	m_Orientation[9] = axisZ.y;
	m_Orientation[10] = axisZ.z;

	return m_Orientation;
}

void RigidBody::Impulse(const CU::Vector3f& anImpulseVector)
{
	myBody->applyForce(btVector3(anImpulseVector.x, anImpulseVector.y, anImpulseVector.z), btVector3(0.f,0.f,0.f));
}

CU::Vector3f RigidBody::GetLinearVelocity()
{
	return CU::Vector3f(myBody->getLinearVelocity().getX(), myBody->getLinearVelocity().getY(), myBody->getLinearVelocity().getZ());
}
//
//void RigidBody::UpdateOrientation(const ControllerState& controller_state)
//{
//	m_CenterPoint.y -= ((float)controller_state.m_ThumbRY / SHRT_MAX) * 0.01f;
//	m_CenterPoint.x += ((float)controller_state.m_ThumbRX / SHRT_MAX) * 0.01f;
//
//	m_CenterPoint.y = fmaxf(fminf(1.57f, m_CenterPoint.y), -1.57f);
//
//	m_Pitch = CU::Quaternion(CU::Vector3f(1.f, 0.f, 0.f), m_CenterPoint.y);
//	m_Yaw = CU::Quaternion(CU::Vector3f(0.f, 1.f, 0.f), m_CenterPoint.x);
//}