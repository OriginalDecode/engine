#include "stdafx.h"
#include "AABB.h"
#include <Randomizer.h>

namespace cl
{
	bool AABB::Initiate(const CU::Matrix44f* orientation, CU::Vector3f width_height_depth)
	{
		m_Orientation = orientation;
		m_WidthHeightDepth = width_height_depth + 0.1f;
		m_ColorValue = RANDOM(0.f, 1.f);
		return true;
	}

	void AABB::Update(const CU::Matrix44f& orientation)
	{
		CU::Vector3f middle_man = m_WidthHeightDepth;
		m_PointsOrientation[0].SetPosition(CU::Vector3f(-middle_man.x, -middle_man.y, -middle_man.z));
		m_PointsOrientation[1].SetPosition(CU::Vector3f(middle_man.x, -middle_man.y, -middle_man.z));
		m_PointsOrientation[2].SetPosition(CU::Vector3f(middle_man.x, -middle_man.y, middle_man.z));
		m_PointsOrientation[3].SetPosition(CU::Vector3f(-middle_man.x, -middle_man.y, middle_man.z));
		m_PointsOrientation[4].SetPosition(CU::Vector3f(-middle_man.x, middle_man.y, middle_man.z));
		m_PointsOrientation[5].SetPosition(CU::Vector3f(middle_man.x, middle_man.y, middle_man.z));
		m_PointsOrientation[6].SetPosition(CU::Vector3f(middle_man.x, middle_man.y, -middle_man.z));
		m_PointsOrientation[7].SetPosition(CU::Vector3f(-middle_man.x, middle_man.y, -middle_man.z));

		if (!m_HasIntersected)
		{
			m_Points[0].color = CU::Vector4f(m_ColorValue, m_ColorValue, m_ColorValue, 1);
			m_Points[1].color = CU::Vector4f(m_ColorValue, m_ColorValue, m_ColorValue, 1);
			m_Points[2].color = CU::Vector4f(m_ColorValue, m_ColorValue, m_ColorValue, 1);
			m_Points[3].color = CU::Vector4f(m_ColorValue, m_ColorValue, m_ColorValue, 1);
			m_Points[4].color = CU::Vector4f(m_ColorValue, m_ColorValue, m_ColorValue, 1);
			m_Points[5].color = CU::Vector4f(m_ColorValue, m_ColorValue, m_ColorValue, 1);
			m_Points[6].color = CU::Vector4f(m_ColorValue, m_ColorValue, m_ColorValue, 1);
			m_Points[7].color = CU::Vector4f(m_ColorValue, m_ColorValue, m_ColorValue, 1);
		}
		else
		{
			for (s32 i = 0; i < array_size; i++)
			{
				m_Points[i].color = m_CollisionColor;
			}
		}



		for (s32 i = 0; i < 8; i++)
		{
			m_PointsOrientation[i] *= orientation;
			m_Points[i].position = m_PointsOrientation[i].GetPosition();
		}



		m_Commands.RemoveAll();
		m_Commands.Add(RenderCommand(eType::LINE_Z_ENABLE, m_Points[0], m_Points[1]));
		m_Commands.Add(RenderCommand(eType::LINE_Z_ENABLE, m_Points[1], m_Points[2]));
		m_Commands.Add(RenderCommand(eType::LINE_Z_ENABLE, m_Points[2], m_Points[3]));
		m_Commands.Add(RenderCommand(eType::LINE_Z_ENABLE, m_Points[3], m_Points[4]));
		m_Commands.Add(RenderCommand(eType::LINE_Z_ENABLE, m_Points[4], m_Points[5]));
		m_Commands.Add(RenderCommand(eType::LINE_Z_ENABLE, m_Points[5], m_Points[6]));
		m_Commands.Add(RenderCommand(eType::LINE_Z_ENABLE, m_Points[6], m_Points[7]));
		m_Commands.Add(RenderCommand(eType::LINE_Z_ENABLE, m_Points[7], m_Points[4]));
		m_Commands.Add(RenderCommand(eType::LINE_Z_ENABLE, m_Points[7], m_Points[0]));
		m_Commands.Add(RenderCommand(eType::LINE_Z_ENABLE, m_Points[0], m_Points[3]));
		m_Commands.Add(RenderCommand(eType::LINE_Z_ENABLE, m_Points[1], m_Points[6]));
		m_Commands.Add(RenderCommand(eType::LINE_Z_ENABLE, m_Points[2], m_Points[5]));

	}

	bool AABB::Intersect(CU::Vector4f position)
	{
		m_HasIntersected = false;

		if (position.x >= m_Orientation->GetPosition().x - m_WidthHeightDepth.x &&
			position.x <= m_Orientation->GetPosition().x + m_WidthHeightDepth.x &&
			position.y >= m_Orientation->GetPosition().y - m_WidthHeightDepth.y &&
			position.y <= m_Orientation->GetPosition().y + m_WidthHeightDepth.y &&
			position.z >= m_Orientation->GetPosition().z - m_WidthHeightDepth.z &&
			position.z <= m_Orientation->GetPosition().z + m_WidthHeightDepth.z)
		{
			m_HasIntersected = true;
			return true;
		}
		return false;
	}

	bool AABB::Intersect(CU::Vector3f pos1, CU::Vector3f pos2)
	{
		return true;
	}

};