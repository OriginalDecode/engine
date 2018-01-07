#pragma once



#include "../Include/bullet/LinearMath/btVector3.h"
#include "CommonLib/Math/Matrix/Matrix44.h"

class btVector3Ext : public btVector3
{
public:
	btVector3Ext(CU::Vector4f vec);
	btVector3& operator=(const CU::Vector4f& vec);
};


namespace btu
{
	btVector3 ConvertVector(const CU::Vector3f& vec3);
	CU::Vector3f ConvertVector(const btVector3& btVec3);
};
