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
		AABB() = default;
		bool Initiate(const CU::Matrix44f* orientation, CU::Vector3f width_height_depth);
		
		u32 GetEntity() const { return m_Entity; }
		const CU::GrowingArray<RenderCommand>& GetCommands() { return m_Commands; }
		void Update(const CU::Matrix44f& orientation);
		bool Intersect(CU::Vector4f position);
		bool Intersect(CU::Vector3f pos1, CU::Vector3f pos2);
	private:
		static const s32 array_size = 8;

		SLinePoint m_Points[array_size];
		CU::Matrix44f m_PointsOrientation[array_size];
		const CU::Matrix44f* m_Orientation;
		CU::Vector4f m_CollisionColor = CU::Vector4f(1.f, 0.f, 0.f, 1.f);
		CU::Vector3f m_WidthHeightDepth;
		float m_ColorValue = 0.f;


		u32 m_Entity;
		CU::GrowingArray<RenderCommand> m_Commands;
		bool m_HasIntersected = false;
	};
};