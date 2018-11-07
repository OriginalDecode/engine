#include "stdafx.h"
#include "AABB.h"
#include <Randomizer.h>

namespace cl
{
	AABB::AABB(float width, float height, float depth, CU::Vector3f center)
		: m_Position(center)
	{
		m_MinSize.x = m_Position.x - width;
		m_MinSize.y = m_Position.y - height;
		m_MinSize.z = m_Position.z - depth;

		m_MaxSize.x = m_Position.x + width;
		m_MaxSize.y = m_Position.y + height;
		m_MaxSize.z = m_Position.z + depth;


	}

	bool AABB::Intersect(const CU::Vector4f& position)
	{
		if (position.x > m_MaxSize.x) return false;
		if (position.x < m_MinSize.x) return false;

		if (position.y > m_MaxSize.y) return false;
		if (position.y < m_MinSize.y) return false;

		if (position.z > m_MaxSize.z) return false;
		if (position.z < m_MinSize.z) return false;

		return true;
	}

};