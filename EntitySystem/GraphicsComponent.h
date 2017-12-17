#pragma once
#include "BaseComponent.h"
#include <Math/Vector/Vector.h>
#include <Engine/engine_shared.h>
#include <CommonLib/DataStructures/GrowingArray.h>
#include <Math/Matrix/Matrix.h>

#include <CommonLib/DataStructures/Hashmap/Hash.h>



struct ModelInstance
{
	template <typename Writer>
	void Serialize(Writer& writer) const;

	static void Deserialize(const rapidjson::Value& json_value, ModelInstance& instance);
	static ModelInstance Deserialize(const rapidjson::Value& json_value);
	//void Deserialize(const rapidjson::Value& json_value);

	std::string m_Filename;
	std::string m_MaterialFile;

	u64 m_ModelID;
	u64 m_MaterialKey;

	CU::Matrix44f m_Orientation;

	CU::Vector4f m_Scale;
	CU::Vector4f m_Rotation;
	CU::Vector4f m_MinPos;
	CU::Vector4f m_MaxPos;

	bool m_RenderWireframe = false;
	bool m_Shadowed = true;


};


struct GraphicsComponent : public BaseComponent
{

	template <typename Writer>
	void Serialize(Writer& writer) const;

	void Deserialize(const rapidjson::Value& json_value);

	CU::GrowingArray<ModelInstance> m_Instances;

	CU::Matrix44f m_Orientation;
	CU::Vector4f m_Scale = { 1.f, 1.f, 1.f, 1.f };
	CU::Vector4f m_Rotation;
	CU::Vector4f m_MinPos;
	CU::Vector4f m_MaxPos;

	bool m_RenderWireframe = false;
	bool m_Shadowed = true;
};



template <typename Writer>
void GraphicsComponent::Serialize(Writer& writer) const
{
	writer.StartObject();
	writer.String("component_type");
	writer.String("graphics");

	writer.String("instances");
	writer.StartArray();
	for (const ModelInstance& i : m_Instances)
	{
		i.Serialize(writer);
	}
	writer.EndArray();

	writer.EndObject();

}


template <typename Writer>
void ModelInstance::Serialize(Writer& writer) const
{
	writer.StartObject();

	writer.String("model_file");
	writer.String(m_Filename.c_str(), (rapidjson::SizeType)m_Filename.length());
	//writer.String("model_key");
	//writer.Number(m_ModelID);

	writer.String("material_file");
	writer.String(m_MaterialFile.c_str(), (rapidjson::SizeType)m_MaterialFile.length());
	//writer.String("material_hash");
	//writer.Number(m_MaterialKey);

	auto& position = m_Orientation.GetPosition();

	writer.String("relative_position");
	writer.StartArray();
	writer.Double(position.x);
	writer.Double(position.y);
	writer.Double(position.z);
	writer.EndArray();


	writer.String("relative_scale");
	CU::Vector4f scale = m_Orientation.GetScale();
	writer.StartArray();
	writer.Double(scale.x);
	writer.Double(scale.y);
	writer.Double(scale.z);
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

	writer.String("shadowed");
	writer.Bool(m_Shadowed);

	writer.EndObject();
}