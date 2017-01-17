#pragma once
#include "CollisionObject.h"
class btGhostObject;

class GhostObject : public CollisionObject
{
public:
	GhostObject() = default;
	btGhostObject* InitAsBox(eCollisionShape shape, float width, float height, float depth, CU::Vector3f position);
	btGhostObject* InitAsBox(eCollisionShape shape, CU::Vector3f size, CU::Vector3f position);
	btGhostObject* InitAsSphere(eCollisionShape shape, float radius, CU::Vector3f position);

private:
	btGhostObject* m_GhostObject = nullptr;
};

