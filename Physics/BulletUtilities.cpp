#include "stdafx.h"
#include "BulletUtilities.h"

namespace bullet_utilities
{
	btVector3 ConvertVector(const CU::Vector3f& vec3)
	{
		btVector3 return_vector;
		return_vector.setX(vec3.x);
		return_vector.setY(vec3.y);
		return_vector.setZ(vec3.z);

		return return_vector;
	}

};