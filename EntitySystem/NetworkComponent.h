#pragma once
#include "ControllerComponent.h"
#include <objbase.h>
struct NetworkComponent final: public ControllerComponent
{
	GUID m_GUID;
	GUID m_Owner;

	void Serialize(JsonWriter& writer) const override;
	void Deserialize(const rapidjson::Value& value) override;
};
