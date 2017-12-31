#pragma once
#include "BaseComponent.h"
struct ControllerComponent : public BaseComponent
{
	virtual void Serialize(JsonWriter& writer) const = 0;
	virtual void Deserialize(const rapidjson::Value& value) = 0;

};
