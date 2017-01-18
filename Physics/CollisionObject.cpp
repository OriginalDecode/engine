#include "stdafx.h"
#include "CollisionObject.h"

void CollisionObject::SetEntity(s32 entity_id)
{
	m_EntityID = entity_id;
}

s32 CollisionObject::GetEntity() const
{
	return m_EntityID;
}

