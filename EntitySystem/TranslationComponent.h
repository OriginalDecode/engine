#pragma once
#include "Math/Matrix/Matrix44.h"
#include "BaseComponent.h"
struct TranslationComponent : public BaseComponent
{
	CU::Matrix44f myOrientation;

	template<typename Writer>
	void Serialize(Writer& writer) const;

	void Deserialize(const rapidjson::Value& value);
};

template<typename Writer>
void TranslationComponent::Serialize(Writer& writer) const
{
	writer.StartObject();
	writer.String("component_type");
	writer.String("translation");

	CU::Vector3f position = myOrientation.GetPosition();
	writer.String("position");
	writer.StartArray();
	writer.Double(position.x);
	writer.Double(position.y);
	writer.Double(position.z);
	writer.EndArray();




	const float rotation_x = myOrientation.GetXRotation();
	const float rotation_y = myOrientation.GetYRotation();
	const float rotation_z = myOrientation.GetZRotation();

	writer.String("rotation");
	writer.StartArray();
	writer.Double(rotation_x);
	writer.Double(rotation_y);
	writer.Double(rotation_z);
	writer.EndArray();

	writer.EndObject();
}
