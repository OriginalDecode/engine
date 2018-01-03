#include "TranslationComponent.h"
#include <CommonLib/Math/Vector/Vector.h>

void TranslationComponent::Serialize(JsonWriter& writer) const
{
	writer.SetMaxDecimalPlaces(3);

	CU::Vector3f position = m_Orientation.GetPosition();
	writer.String("position");
	writer.StartArray();
	writer.Double(position.x);
	writer.Double(position.y);
	writer.Double(position.z);
	writer.EndArray();

	const CU::Vector4f up = CU::Math::GetNormalized(m_Orientation.GetUp());
	const CU::Vector4f fwd = CU::Math::GetNormalized(m_Orientation.GetForward());

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

void TranslationComponent::Deserialize(const rapidjson::Value& value)
{
	auto& position = value["position"].GetArray();
	m_Orientation.SetPosition({ (float)position[0].GetDouble(), (float)position[1].GetDouble(), (float)position[2].GetDouble(), 1.f });

	auto& up = value["up"].GetArray();

	const float up_x = (float)up[0].GetDouble();
	const float up_y = (float)up[1].GetDouble();
	const float up_z = (float)up[2].GetDouble();

	auto& fwd = value["forward"].GetArray();

	const float fwd_x = (float)fwd[0].GetDouble();
	const float fwd_y = (float)fwd[1].GetDouble();
	const float fwd_z = (float)fwd[2].GetDouble();

	CU::Vector4f vec_up = { up_x, up_y, up_z, 0 };
	CU::Vector4f vec_fwd = { fwd_x, fwd_y, fwd_z, 0 };

	CU::Vector4f vec_rgt = CU::Math::Cross(vec_up, vec_fwd);
	vec_rgt.w = 0;
	
	m_Orientation.SetUp(vec_up);
	m_Orientation.SetForward(vec_fwd);
	m_Orientation.SetRight(vec_rgt);

}

void TranslationComponent::SetPosition(const CU::Vector3f& pos)
{
	m_Orientation.SetPosition(pos);
}

void TranslationComponent::SetOrientation(const CU::Matrix44f& orientation)
{
	m_Orientation = orientation;
}

const CU::Matrix44f& TranslationComponent::GetOrientation() const
{

	return m_Orientation;
}

void TranslationComponent::OnNotify(eNetMessageType type, void* pData) //This is breaking conventions
{
	if (type != eNetMessageType::NET_ENTITY_DATA)
		return;

	CU::Vector3f pos;
	memcpy(&pos, pData, sizeof(float) * 3);
	SetPosition(pos);
	m_UpdatedOnNetwork = true;
}
