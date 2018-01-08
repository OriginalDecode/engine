#pragma once
#include <CommonLib/Math/Matrix/Matrix.h>
#include "BaseComponent.h"
#include <CommonLib/Utilities.h>
struct TranslationComponent : public BaseComponent, public NetReplicate
{
	void Serialize(JsonWriter& writer) const override;
	void Deserialize(const rapidjson::Value& value) override;

	void OnNotify(eNetMessageType type, void* pData) override;
	
	void SetPosition(const CU::Vector3f& pos);
	void SetOrientation(const CU::Matrix44f& orientation);
	const CU::Matrix44f& GetOrientation() const;

	bool m_UpdatedOnNetwork = false;
	CU::Matrix44f m_Orientation;
	CU::Matrix44f m_Scale;
private:

};

