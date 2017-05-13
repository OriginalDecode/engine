#pragma once
#include <string>
#include "BaseComponent.h"
#include "Math/Vector/Vector.h"
#include "Math/Matrix/Matrix.h"


enum class eLightType
{
	ePOINTLIGHT,
	eSPOTLIGHT,
};

struct LightComponent : public BaseComponent
{

	eLightType myType;
	CU::Vector3f color;

	union
	{
		float intensity;
		float angle;
	};

	float range = 0.f;
	CU::Vector3f direction;
	CU::Matrix44f orientation;
	int m_LightID = 0;
};