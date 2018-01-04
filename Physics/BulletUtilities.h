#pragma once
#include "../include/Bullet_Physics/LinearMath/btVector3.h"
#include "CommonLib/Math/Matrix/Matrix44.h"
namespace btu
{
	btVector3 ConvertVector(const CU::Vector3f& vec3);
	CU::Vector3f ConvertVector(const btVector3& btVec3);

};
