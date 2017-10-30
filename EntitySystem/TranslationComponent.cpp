#include "TranslationComponent.h"

void TranslationComponent::Deserialize(const rapidjson::Value& value)
{
	auto& position = value["position"].GetArray();
	myOrientation.SetPosition({ (float)position[0].GetDouble(), (float)position[1].GetDouble(), (float)position[2].GetDouble(), 1.f });

	auto& rotation = value["rotation"].GetArray();
	myOrientation = CU::Matrix44f::CreateRotateAroundX((float)rotation[0].GetDouble() * (3.14f / 180.f)) * myOrientation;
	myOrientation = CU::Matrix44f::CreateRotateAroundY((float)rotation[1].GetDouble() * (3.14f / 180.f)) * myOrientation;
	myOrientation = CU::Matrix44f::CreateRotateAroundZ((float)rotation[2].GetDouble() * (3.14f / 180.f)) * myOrientation;

}
