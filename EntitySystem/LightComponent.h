#pragma once
#include <string>
#include "BaseComponent.h"
#include <CommonLib/Math/Vector/Vector.h>
#include <CommonLib/Math/Matrix/Matrix.h>

enum class eLightType
{
	ePOINTLIGHT,
	eSPOTLIGHT,
};

struct LightComponent : public BaseComponent
{

	void Serialize(JsonWriter& writer) const override;
	void Deserialize(const rapidjson::Value& value) override;

	eLightType myType;
	CU::Vector3f color;
	float intensity = 1.f;
	float angle = 0.f;
	float range = 0.f;
	int m_LightID = 0;
			

};
