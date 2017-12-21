#pragma once
#include "CommonLib/Math/Matrix/Matrix44.h"
#include "BulletUtilities.h"
#include <standard_datatype.hpp>
class btCollisionShape;
struct btDefaultMotionState;

class CollisionObject
{
public:
	virtual ~CollisionObject() { };
	void SetEntity(s32 entity_id);
	s32 GetEntity() const;
	
protected:




	s32 m_EntityID;
	btCollisionShape* m_Shape			= nullptr;
	btDefaultMotionState* m_MotionState = nullptr;
	CU::Matrix44f m_Orientation;
};

