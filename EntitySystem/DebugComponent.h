#pragma once
#include <Math/Matrix/Matrix44.h>
#include "BaseComponent.h"

struct DebugComponent final : public BaseComponent
{
	~DebugComponent() { }
	void* m_Dweller = nullptr;

	CU::Vector4f m_WHD;
	CU::Vector4f m_MinPoint;
	CU::Vector4f m_MaxPoint;

	CU::Vector4f debugColor = { 255.f, 255.f, 255.f, 255.f };
	int m_ComponentFlags = 0;
	bool m_DirtyFlag = false;

	void Serialize(JsonWriter& writer) const override;
	void Deserialize(const rapidjson::Value& value) override;

};
