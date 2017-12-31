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
	color.x = (float)in_color[0].GetDouble();
	color.y = (float)in_color[1].GetDouble();
	color.z = (float)in_color[2].GetDouble();

	intensity = (float)value[c_Intensity].GetDouble();
	angle = (float)value[c_Angle].GetDouble();
	range = (float)value[c_Range].GetDouble();
	
}

void LightComponent::Serialize(JsonWriter& writer) const
{
	writer.StartObject();
	writer.String("component_type");
	writer.String("light");

	writer.String(c_LightType);
	if (myType == eLightType::eSPOTLIGHT)
		writer.Uint((u32)eLightType::eSPOTLIGHT);
	else if (myType == eLightType::ePOINTLIGHT)
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
