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
	float intensity = 1.f;
	float angle = 0.f;
	float range = 0.f;
	int m_LightID = 0;

	template<typename Writer>
	void Serialize(Writer& writer) const;

};

template<typename Writer>
void LightComponent::Serialize(Writer& writer) const
{
	writer.StartObject();

	writer.String("lightype");
	if (myType == eLightType::eSPOTLIGHT)
	{
		writer.Uint((u32)eLightType::eSPOTLIGHT);
	}
	else if(myType == eLightType::ePOINTLIGHT)
	{
		writer.Uint((u32)eLightType::ePOINTLIGHT);
	}
	
	writer.String("color");
	writer.StartArray();
	writer.Double(color.x);
	writer.Double(color.y);
	writer.Double(color.z);
	writer.EndArray();

	writer.String("intensity");
	writer.Double(intensity);
	
	writer.String("angle");
	writer.Double(angle);
	
	writer.String("range");
	writer.Double(range);
	
	writer.EndObject();
}
