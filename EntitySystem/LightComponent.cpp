#include "LightComponent.h"

const char* c_LightType = "lighttype";
const char* c_Intensity = "intensity";
const char* c_Angle = "angle";
const char* c_Range = "range";
const char* c_Color = "color";

void LightComponent::Deserialize(const rapidjson::Value& value)
{
	myType = (eLightType)value[c_LightType].GetUint();

	auto& in_color = value[c_Color].GetArray();
	color.x = (float)in_color[0].GetDouble() * 255;
	color.y = (float)in_color[1].GetDouble() * 255;
	color.z = (float)in_color[2].GetDouble() * 255;

	intensity = (float)value[c_Intensity].GetDouble();
	angle = (float)value[c_Angle].GetDouble() / 2.f;
	range = (float)value[c_Range].GetDouble();
	
}

void LightComponent::Serialize(JsonWriter& writer) const
{

	writer.String(c_LightType);
	if (myType == eLightType::eSPOTLIGHT)
		writer.Uint((uint32)eLightType::eSPOTLIGHT);
	else if (myType == eLightType::ePOINTLIGHT)
		writer.Uint((uint32)eLightType::ePOINTLIGHT);

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
