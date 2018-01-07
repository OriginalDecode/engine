#include "stdafx.h"
#include "BulletUtilities.h"

namespace btu
{
	btVector3 ConvertVector(const CU::Vector3f& vec3)
	{
		btVector3 return_vector;
		return_vector.setX(vec3.x);
		return_vector.setY(vec3.y);
		return_vector.setZ(vec3.z);

		return return_vector;
	}

	CU::Vector3f ConvertVector(const btVector3& btVec3)
	{
		CU::Vector3f return_vector;
		return_vector.x = btVec3.getX();
		return_vector.y = btVec3.getY();
		return_vector.z = btVec3.getZ();

		return return_vector;
	}

};

btVector3& btVector3Ext::operator=(const CU::Vector4f& vec)
{
	m_floats[0] = vec.x;
	m_floats[1] = vec.y;
	m_floats[2] = vec.z;
	return *this;
}

btVector3Ext::btVector3Ext(CU::Vector4f vec)
{
	*this = vec;
}
