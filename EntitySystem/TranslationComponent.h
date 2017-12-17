#pragma once
#include "Math/Matrix/Matrix44.h"
#include "BaseComponent.h"
#include <CommonLib/Utilities.h>
struct TranslationComponent : public BaseComponent
{
	CU::Matrix44f myOrientation;

	template<typename Writer>
	void Serialize(Writer& writer) const;

	void Deserialize(const rapidjson::Value& value);
private:

};

template<typename Writer>
void TranslationComponent::Serialize(Writer& writer) const
{
	writer.SetMaxDecimalPlaces(3);
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

	const CU::Vector4f up = CU::Math::GetNormalized(myOrientation.GetUp());
	const CU::Vector4f fwd = CU::Math::GetNormalized(myOrientation.GetForward());
	
	writer.String("up");
	writer.StartArray();
	writer.Double(up.x);
	writer.Double(up.y);
	writer.Double(up.z);
	writer.EndArray();


	writer.String("forward");
	writer.StartArray();
	writer.Double(fwd.x);
	writer.Double(fwd.y);
	writer.Double(fwd.z);
	writer.EndArray();

	writer.EndObject();
}
