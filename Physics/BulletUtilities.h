#pragma once
#include "../include/Bullet_Physics/LinearMath/btVector3.h"
#include "CommonLib/Math/Matrix/Matrix44.h"
namespace bullet_utilities
{
	btVector3 ConvertVector(const CU::Vector3f& vec3);
};
