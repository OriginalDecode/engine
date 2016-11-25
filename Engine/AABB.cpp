#include "stdafx.h"
#include "AABB.h"
#include <Randomizer.h>

namespace cl
{
	AABB::AABB()
	{
		m_ColorValue = RANDOM(0.f, 255.f);
	}

	bool AABB::Initiate(u32 entity_id, CU::Vector3f width_height_depth)
	{
		m_Entity = entity_id;
		m_WidthHeightDepth = width_height_depth;
		m_WidthHeightDepth /= 2.f;
		return true;
	}

	void AABB::Update(const CU::Vector3f& whd, const CU::Matrix44f& orientation)
	{
		CU::Vector3f middle_man = whd + 0.1f;
		
		m_PointsOrientation[0].SetPosition(CU::Vector3f(0.f, 0.f, 0.f) - middle_man);
		m_PointsOrientation[1].SetPosition(CU::Vector3f(middle_man.x, -middle_man.y, -middle_man.z));
		m_PointsOrientation[2].SetPosition(CU::Vector3f(middle_man.x, -middle_man.y, middle_man.z));
		m_PointsOrientation[3].SetPosition(CU::Vector3f(-middle_man.x, -middle_man.y, middle_man.z));
		m_PointsOrientation[4].SetPosition(CU::Vector3f(-middle_man.x, middle_man.y, middle_man.z));
		m_PointsOrientation[5].SetPosition(CU::Vector3f(middle_man.x, middle_man.y, middle_man.z));
		m_PointsOrientation[6].SetPosition(CU::Vector3f(middle_man.x, middle_man.y, -middle_man.z));
		m_PointsOrientation[7].SetPosition(CU::Vector3f(-middle_man.x, middle_man.y, -middle_man.z));

		m_Points[0].color = CU::Vector4f(m_ColorValue, m_ColorValue, m_ColorValue, 1);
		m_Points[1].color = CU::Vector4f(m_ColorValue, m_ColorValue, m_ColorValue, 1);
		m_Points[2].color = CU::Vector4f(m_ColorValue, m_ColorValue, m_ColorValue, 1);
		m_Points[3].color = CU::Vector4f(m_ColorValue, m_ColorValue, m_ColorValue, 1);
		m_Points[4].color = CU::Vector4f(m_ColorValue, m_ColorValue, m_ColorValue, 1);
		m_Points[5].color = CU::Vector4f(m_ColorValue, m_ColorValue, m_ColorValue, 1);
		m_Points[6].color = CU::Vector4f(m_ColorValue, m_ColorValue, m_ColorValue, 1);
		m_Points[7].color = CU::Vector4f(m_ColorValue, m_ColorValue, m_ColorValue, 1);

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

};