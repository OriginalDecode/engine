#include "stdafx.h"
#include "GhostObject.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

GhostObject::~GhostObject()
{
	delete m_GhostObject;
	m_GhostObject = nullptr;

	delete m_Shape;
	m_Shape = nullptr;

	/*delete m_MotionState;
	m_MotionState = nullptr;*/
}

btGhostObject* GhostObject::InitAsBox(float width, float height, float depth, CU::Vector3f position)
{
	m_GhostObject = new btGhostObject;
	m_Shape = new btBoxShape(btVector3(width, height, depth));
	m_GhostObject->setCollisionShape(m_Shape);
	
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(bullet_utilities::ConvertVector(position));

	m_GhostObject->setWorldTransform(transform);

	return m_GhostObject;
}

btGhostObject* GhostObject::InitAsBox(CU::Vector3f size, CU::Vector3f position)
{
	return InitAsBox(size.x, size.y, size.z, position);
}

btGhostObject* GhostObject::InitAsSphere(float radius, CU::Vector3f position)
{
	m_GhostObject = new btGhostObject;
	m_Shape = new btSphereShape(radius);
	m_GhostObject->setCollisionShape(m_Shape);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(bullet_utilities::ConvertVector(position));

	m_GhostObject->setWorldTransform(transform);
	return m_GhostObject;
}

void GhostObject::SetPosition(const CU::Vector3f& position)
{
	m_GhostObject->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), bullet_utilities::ConvertVector(position)));
	m_Orientation.SetPosition(position);
}

const CU::Matrix44f& GhostObject::GetOrientation() const
{
	/*myWorldTranslation->getOpenGLMatrix(&m_Orientation.myMatrix[0]);

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

	return m_Orientation;*/
	return m_Orientation;
}
