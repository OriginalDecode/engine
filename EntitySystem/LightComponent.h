#pragma once
#include <string>
#include "BaseComponent.h"
#include "Math/Vector/Vector.h"

enum class eLightType
{
	ePOINTLIGHT,
	eSPOTLIGHT,
};

struct SLightComponent : public SBaseComponent
{
	~SLightComponent();

	eLightType myType;
	CU::Vector3f color;
	float intensity = 0.f;
	float range = 0.f;

} typedef LightComponent;