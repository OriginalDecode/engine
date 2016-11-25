#pragma once
#include <Math/Vector/Vector.h>
#include <Math/Matrix/Matrix.h>
#include "Line3D.h"
#include <DataStructures/GrowingArray.h>
#include "RenderCommand.h"
#include "snowblind_shared.h"
#include "standard_datatype.hpp"
namespace cl
{
	class AABB
	{
	public:
		AABB();
		bool Initiate(u32 entity_id, CU::Vector3f width_height_depth);
		
		u32 GetEntity() const { return m_Entity; }
		const CU::GrowingArray<RenderCommand>& GetCommands() { return m_Commands; }
		void Update(const CU::Vector3f& whd, const CU::Matrix44f& orientation);
	private:
		u32 m_Entity;
		float m_ColorValue = 0.f;
		CU::Vector3f m_WidthHeightDepth;
		
		SLinePoint m_Points[8];
		CU::Matrix44f m_PointsOrientation[8];


		CU::GrowingArray<RenderCommand> m_Commands;
		Ticket_Mutex m_Mutex;
	};
};