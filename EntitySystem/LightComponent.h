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

	void Deserialize(const rapidjson::Value& value);

private:
	const char* c_LightType = "lighttype";
	const char* c_Intensity = "intensity";
	const char* c_Angle = "angle";
	const char* c_Range = "range";
	const char* c_Color = "color";
		

};

template<typename Writer>
void LightComponent::Serialize(Writer& writer) const
{
	writer.StartObject();
	writer.String("component_type");
	writer.String("light");

	writer.String(c_LightType);
	if (myType == eLightType::eSPOTLIGHT)
		writer.Uint((u32)eLightType::eSPOTLIGHT);
	else if(myType == eLightType::ePOINTLIGHT)
		writer.Uint((u32)eLightType::ePOINTLIGHT);
	
	writer.String(c_Color);
	writer.StartArray();
	writer.Double(color.x);
	writer.Double(color.y);
	writer.Double(color.z);
	writer.EndArray();

	writer.String(c_Intensity);
	writer.Double(intensity);
	
	writer.String(c_Angle);
	writer.Double(angle);
	
	writer.String(c_Range);
	writer.Double(range);
	
	writer.EndObject();
}
