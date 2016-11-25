#pragma once
#include "AABB.h"
#include "BaseComponent.h"
struct AABBComponent : public BaseComponent
{
	cl::AABB m_AABB;
	bool m_IsActive = true;
};