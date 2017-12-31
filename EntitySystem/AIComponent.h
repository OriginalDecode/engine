#pragma once
#include "ControllerComponent.h"

struct AIComponent final: public ControllerComponent
{

	void Serialize(JsonWriter& writer) const override;
	void Deserialize(const rapidjson::Value& value) override;

};
