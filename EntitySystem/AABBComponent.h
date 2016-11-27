#pragma once
#include "AABB.h"
#include "BaseComponent.h"
#include "../Physics/RigidBody.h"

struct AABBComponent : public BaseComponent
{
	~AABBComponent(){ delete m_Body; m_Body = nullptr; }
	cl::AABB m_AABB;
	CRigidBody* m_Body = nullptr;
	bool m_IsActive = true;
};