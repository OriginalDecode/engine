#include "TranslationComponent.h"

void TranslationComponent::Deserialize(const rapidjson::Value& value)
{
	auto& position = value["position"].GetArray();
	myOrientation.SetPosition({ (float)position[0].GetDouble(), (float)position[1].GetDouble(), (float)position[2].GetDouble(), 1.f });

	auto& rotation = value["rotation"].GetArray();

	float x = (float)rotation[0].GetDouble();
	float y = (float)rotation[1].GetDouble();
	float z = (float)rotation[2].GetDouble();

	myOrientation = CU::Matrix44f::CreateRotateAroundX(z) * myOrientation;
	myOrientation = CU::Matrix44f::CreateRotateAroundY(y) * myOrientation;
	myOrientation = CU::Matrix44f::CreateRotateAroundZ(x) * myOrientation;

}
