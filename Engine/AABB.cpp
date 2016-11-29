#include "stdafx.h"
#include "AABB.h"
#include <Randomizer.h>

namespace cl
{
	bool AABB::Initiate(u32 /*entity_id*/, const CU::Matrix44f* orientation, CU::Vector3f width_height_depth)
	{
		m_Orientation = orientation;
		m_WidthHeightDepth = width_height_depth + 0.1f;

		m_ColorValue.r = 1;//RANDOM(0.f, 1.f);
		m_ColorValue.g = 1;//RANDOM(0.f, 1.f);
		m_ColorValue.b = 1;//RANDOM(0.f, 1.f);
		m_ColorValue.a = 1;

		m_IntersectedColor = CU::Vector4f(1.f, 0.f, 0.f, 1.f);
		return true;
	}

	void AABB::Update()
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
			m_Points[0].color = m_ColorValue;
			m_Points[1].color = m_ColorValue;
			m_Points[2].color = m_ColorValue;
			m_Points[3].color = m_ColorValue;
			m_Points[4].color = m_ColorValue;
			m_Points[5].color = m_ColorValue;
			m_Points[6].color = m_ColorValue;
			m_Points[7].color = m_ColorValue;	
		}
		else
		{
			for (s32 i = 0; i < array_size; i++)
			{
				m_Points[i].color = m_IntersectedColor;
			}
		}

		for (s32 i = 0; i < 8; i++)
		{
			m_PointsOrientation[i] *= *m_Orientation;
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

	bool AABB::Intersect(CU::Vector4f position, CU::Vector4f direction)
	{
		m_HasIntersected = false;

		CU::Vector4f delta = m_Orientation->GetTranslation() - position;
		CU::Vector4f right = m_Orientation->GetRight();

		float e = CU::Math::Dot(delta, right);
		float f = CU::Math::Dot(direction, right);
		
		float t1 = (e + m_MinPos.x) / f;
		float t2 = (e + m_MaxPos.x) / f;

		if (t1 > t2)
		{
			float t = t1;
			t1 = t2;
			t1 = t;
		}

		float tMin = 0.f;
		float tMax = 100000.f;

		if (t2 < tMax)
			tMax = t2;
		if (t1 > tMin)
			tMin = t1;
		if (tMax < tMin)
			return false;

	
		m_HasIntersected = true;
		return true;
	}

};