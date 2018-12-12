#pragma once
#include "ControllerComponent.h"
#include <standard_datatype.hpp>
#include "../Input/InputHandle.h"
struct InputComponent final : public ControllerComponent
{
	~InputComponent() override { delete m_InputHandle; m_InputHandle = nullptr; }
	int16 m_ID = -1;
	InputHandle* m_InputHandle = nullptr;


	void Serialize(JsonWriter& writer) const override;
	void Deserialize(const rapidjson::Value& value) override;


};
