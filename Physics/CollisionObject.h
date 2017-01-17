#pragma once
#include "CommonLib/Math/Matrix/Matrix44.h"
#include "BulletUtilities.h"
class btCollisionShape;
struct btDefaultMotionState;

enum class eCollisionShape
{
	eBOX,
	eSPHERE,
};

class CollisionObject
{
public:
	CollisionObject() = default;
	virtual ~CollisionObject() = 0;
protected:
	btCollisionShape* m_Shape			= nullptr;
	btDefaultMotionState* m_MotionState = nullptr;
};

