#include "LightComponent.h"

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
