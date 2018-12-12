#include "stdafx.h"
#include "CollisionObject.h"

void CollisionObject::SetEntity(int32 entity_id)
{
	m_EntityID = entity_id;
}

int32 CollisionObject::GetEntity() const
{
	return m_EntityID;
}

