#include "GraphicsComponent.h"
#include <Engine/Engine.h>
#include <Engine/Material.h>


constexpr char* s_RelativeScale = "relative_scale";
constexpr char* s_RelativePos = "relative_position";
constexpr char* s_RelativeRot = "relative_rotation";
constexpr char* s_MaterialFile = "material_file";
constexpr char* s_Shader = "Shaders/debug_pbl_instanced.json"; // This should be read from a file, this should be connected with the material
constexpr char* s_ModelFile = "model_file";
constexpr char* s_Shadowed = "shadowed";
constexpr char* s_Instances = "instances";



void ModelInstance::Deserialize(const rapidjson::Value& json_value, ModelInstance& instance)
{
	instance.m_Filename = json_value[s_ModelFile].GetString();

	instance.m_ModelID = Engine::GetInstance()->LoadModelA(instance.m_Filename, s_Shader, false);//Engine::GetInstance()->LoadModel<Model>(m_Filename, "s", true); //add request???

	instance.m_MaterialFile = json_value[s_MaterialFile].GetString();
	Material* pMaterial = Engine::GetInstance()->GetMaterial(instance.m_MaterialFile.c_str());
	pMaterial->SetEffect(Engine::GetInstance()->GetEffect(s_Shader));
	instance.m_MaterialKey = pMaterial->GetKey();

	if (json_value.FindMember(s_RelativePos) != json_value.MemberEnd())
	{
		auto& pos = json_value[s_RelativePos].GetArray();
		instance.m_Orientation.SetPosition({ (float)pos[0].GetDouble(), (float)pos[1].GetDouble(), (float)pos[2].GetDouble(), 1.f });
	}
	
	if (json_value.FindMember(s_RelativeScale) != json_value.MemberEnd())
	{
		auto& scale = json_value[s_RelativeScale].GetArray();
		instance.m_Scale = { (float)scale[0].GetDouble(), (float)scale[1].GetDouble(), (float)scale[2].GetDouble(), 1.f };
		if (instance.m_Scale.x <= 0.f && instance.m_Scale.y <= 0.f && instance.m_Scale.z <= 0.f)
		{
			instance.m_Scale.x = 1.f;
			instance.m_Scale.y = 1.f;
			instance.m_Scale.z = 1.f;
			instance.m_Scale.w = 1.f;
		}
	}

	if (json_value.FindMember(s_RelativeRot) != json_value.MemberEnd())
	{
		auto& rot = json_value[s_RelativeRot].GetArray();
		instance.m_Rotation = { (float)rot[0].GetDouble(), (float)rot[1].GetDouble(), (float)rot[2].GetDouble(), 1.f };
	}

	if (json_value.FindMember(s_Shadowed) != json_value.MemberEnd())
	{
		instance.m_Shadowed = json_value[s_Shadowed].GetBool();
	}
}

ModelInstance ModelInstance::Deserialize(const rapidjson::Value& json_value)
{
	ModelInstance instance;
	ModelInstance::Deserialize(json_value, instance);
	return instance;
}

void GraphicsComponent::Deserialize(const rapidjson::Value& json_value)
{
	for (const rapidjson::Value& value : json_value[s_Instances].GetArray())
	{
		m_Instances.Add(ModelInstance::Deserialize(value));
	}
}