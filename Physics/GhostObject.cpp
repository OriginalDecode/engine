#include "stdafx.h"
#include "GhostObject.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

btGhostObject* GhostObject::InitAsBox(eCollisionShape shape, float width, float height, float depth, CU::Vector3f position)
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

btGhostObject* GhostObject::InitAsBox(eCollisionShape shape, CU::Vector3f size, CU::Vector3f position)
{
	return InitAsBox(shape, size.x, size.y, size.z, position);
}

btGhostObject* GhostObject::InitAsSphere(eCollisionShape shape, float radius, CU::Vector3f position)
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
