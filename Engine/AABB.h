#pragma once
#include <Math/Vector/Vector.h>
namespace cl
{
	class AABB
	{
	public:
		AABB() = default;
		AABB(float width, float height, float depth, CU::Vector3f center);
		bool Intersect(const CU::Vector4f& position);

	private:
		CU::Vector3f m_MinSize;
		CU::Vector3f m_MaxSize;
		CU::Vector3f m_Position;

	};
};