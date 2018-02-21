#pragma once
#include <Math/Vector/Vector.h>
#include <Math/Matrix/Matrix.h>
#include "Line3D.h"
#include <DataStructures/GrowingArray.h>
#include "RenderCommand.h"
#include "engine_shared.h"
#include "standard_datatype.hpp"
namespace cl
{



	class AABB
	{
	public:
		AABB() = default;
		AABB(float width, float height, float depth, CU::Vector4f center);
		bool Intersect(const CU::Vector4f& position);



	private:
		CU::Vector3f m_MinSize;
		CU::Vector3f m_MaxSize;
		CU::Vector4f m_Position;

	};
};