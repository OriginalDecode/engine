#include "stdafx.h"
#include "RigidBody.h"
#include "PhysicsDefines.h"

#include <Utilities.h>
#include <Engine/engine_shared.h>

#include "BulletFileLoader/bFile.h"
#include "BulletWorldImporter/btBulletWorldImporter.h"
#include "BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h"
#include "btBulletDynamicsCommon.h"


RigidBody::RigidBody()
	: m_IsEnabled(true)
	, m_IsStatic(false)
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
	m_Shape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	m_MotionState = new btDefaultMotionState();
	m_Shape->setMargin(0.025f);
	btRigidBody::btRigidBodyConstructionInfo bodyInfo(0, m_MotionState, m_Shape, btVector3(0, 0, 0));
	//bodyInfo.m_friction = 1.5f;
	//bodyInfo.m_restitution = 1.0f;

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
	myTerminalVelocity.y = cl::CalcTerminalVelocity(myMass, myGravity, myDragCoeff, myCrossSectionArea, myResistanceDensity);

	return myBody;
}

btRigidBody* RigidBody::InitWithMeshCollision(s8* const vertices, s8* const indices, const s32 idx_count, const s32 vtx_count)
{
	const s32 vtx_size = ARRSIZE(vertices);
	s32 faceCount = idx_count / 3;
	s32 vStride = sizeof(CU::Vector3f);
	s32 iStride = sizeof(u32) * 3;


	btScalar* vtx = new btScalar[vtx_size];
	memcpy(&vtx[0], &vertices[0], sizeof(float) * vtx_size);

	s32* locIndices = new s32[idx_count];
	memcpy(&locIndices[0], &indices[0], sizeof(s32) * idx_count);


	btTriangleIndexVertexArray* triangle_list = new btTriangleIndexVertexArray(faceCount, locIndices, iStride, vtx_count, vtx, vStride);
	//btConvexTriangleMeshShape()
	m_Shape = new btBvhTriangleMeshShape(triangle_list, false, btVector3(0, 0, 0), btVector3(1, 1, 1), true);



	btScalar mass = 10.f;
	btVector3 localInertia(0, 0, 0);
	const bool is_dynamic = (mass > 0.f);
	if (is_dynamic)
		m_Shape->calculateLocalInertia(mass, localInertia);

	m_MotionState = new btDefaultMotionState();
	myWorldTranslation = &m_MotionState->m_graphicsWorldTrans;

	btRigidBody::btRigidBodyConstructionInfo bodyInfo(mass, m_MotionState, m_Shape, localInertia);
	myBody = new btRigidBody(bodyInfo);
	//myBody->setGravity(btVector3(0,-myGravity,0));
	myBody->activate();

	delete triangle_list;
	delete locIndices;
	delete vtx;


	return myBody;
}

btRigidBody* RigidBody::InitAsBox(float width, float height, float depth, CU::Vector3f position)
{
	m_Shape = new btBoxShape(btVector3(width, height, depth));
	m_Shape->setMargin(0.025f);
	btVector3 pos = btu::ConvertVector(position);

	m_MotionState = new btDefaultMotionState;
	btScalar mass = 10.f;
	myMass = mass;
	btVector3 local_inertia;
	m_Shape->calculateLocalInertia(mass, local_inertia);
	btRigidBody::btRigidBodyConstructionInfo bodyInfo(mass, m_MotionState, m_Shape, local_inertia);
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
	const btTransform& transform = myBody->getWorldTransform();
	myBody->clearForces();
	btTransform initial;
	initial.setOrigin(btu::ConvertVector(aPosition));
	initial.setRotation(btQuaternion(0, 0, 0, 1));

	myBody->setWorldTransform(initial);
	m_MotionState->setWorldTransform(initial);
	myBody->activate();

	//myBody->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btu::ConvertVector(aPosition) ));
}

void RigidBody::Update(float deltaTime)
{


	if (!myBody)
		return;

	btVector3 linear_velocity = myBody->getLinearVelocity();
	myVelocity.x = linear_velocity.getX();
	myVelocity.y = linear_velocity.getY();
	myVelocity.z = linear_velocity.getZ();

	const float v = 10.f;

	if (myVelocity.x >= v)
		myVelocity.x = v;
	if (myVelocity.x <= -v)
		myVelocity.x = -v;


	if (myVelocity.y >= v)
		myVelocity.y = v;
	if (myVelocity.y <= -v)
		myVelocity.y = -v;

	if (myVelocity.z >= v)
		myVelocity.z = v;
	if (myVelocity.z <= -v)
		myVelocity.z = -v;

	myBody->setLinearVelocity(btVector3(myVelocity.x, myVelocity.y, myVelocity.z));
	//m_MotionState->l
}

btRigidBody* RigidBody::GetBody()
{
	return myBody;
}

const CU::Matrix44f& RigidBody::GetOrientation()
{
	if (!m_Shape)
		return CU::Matrix44f();


	myWorldTranslation->getOpenGLMatrix(&m_Orientation.myMatrix[0]);
	

	/*CU::Vector3f axisX(1.f, 0.f, 0.f);
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
	m_Orientation[10] = axisZ.z;*/

	return m_Orientation;
}

void RigidBody::Impulse(const CU::Vector3f& anImpulseVector)
{

	//btVector3 relPos = hitPoint_in_world_space - rgBody.getCentreOfMassPosition();
	btVector3 rel_pos = btu::ConvertVector(m_Orientation.GetPosition() - CU::Vector3f(0, 0, 1));
	myBody->applyImpulse(btu::ConvertVector(anImpulseVector), rel_pos);
	//myBody->applyForce(btVector3(anImpulseVector.x, anImpulseVector.y, anImpulseVector.z), btVector3(0.f, 0.f, 0.f));
}

CU::Vector3f RigidBody::GetLinearVelocity()
{
	return CU::Vector3f(myBody->getLinearVelocity().getX(), myBody->getLinearVelocity().getY(), myBody->getLinearVelocity().getZ());
}

void RigidBody::SetStatic(bool is_static)
{
	m_IsStatic = is_static;
	if (m_IsStatic)
		myBody->setMassProps(0, btVector3(0, 0, 0));
	else
	{
		myBody->setMassProps(myMass, btVector3(0, 0, 0));
		myBody->setActivationState(ACTIVE_TAG);
	}
}

void RigidBody::SetMass(float mass)
{
	myMass = mass;
	myBody->setMassProps(myMass, btVector3(0, 0, 0));
}

void RigidBody::SerializePhysicsData(unsigned char*& buffer_pointer, int& buffer_size)
{
	btDefaultSerializer* serializer = new btDefaultSerializer;

	serializer->startSerialization();
	myBody->serializeSingleObject(serializer);
	m_Shape->serializeSingleShape(serializer);
	serializer->finishSerialization();

	serializer->getBufferPointer();
	buffer_size = serializer->getCurrentBufferSize();

	buffer_pointer = new unsigned char[buffer_size];
	ZeroMemory(buffer_pointer, buffer_size);
	memcpy(buffer_pointer, serializer->getBufferPointer(), buffer_size);

	delete serializer;
}

btRigidBody* RigidBody::DeserializePhysicsData(char* mem_buffer, int length, const CU::Vector3f& initial_pos)
{
	btBulletWorldImporter* importer = new btBulletWorldImporter;
 	importer->loadFileFromMemory(mem_buffer, length);
 	const int count = importer->getNumCollisionShapes();
 	m_Shape = importer->getCollisionShapeByIndex(0);

	const int numRigidBodies = importer->getNumRigidBodies();
	myBody = (btRigidBody*)importer->getRigidBodyByIndex(0);
	
	myBody->setCollisionShape(m_Shape);

	m_MotionState = new btDefaultMotionState;
	myBody->setMotionState(m_MotionState);
	myWorldTranslation = &m_MotionState->m_graphicsWorldTrans;

	SetPosition(initial_pos);


 	delete importer;
 	return myBody;
}
