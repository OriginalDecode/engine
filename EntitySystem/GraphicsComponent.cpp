#include "GraphicsComponent.h"
#include <Engine/Engine.h>
#include <Engine/Material.h>
void ModelInstance::Deserialize(const rapidjson::Value& json_value)
{
	m_Filename = json_value["model_file"].GetString();	

	m_ModelID = Engine::GetInstance()->LoadModelA(m_Filename, "Shaders/debug_pbl_instanced.json", false);//Engine::GetInstance()->LoadModel<Model>(m_Filename, "s", true); //add request???

	m_MaterialFile = json_value["material_file"].GetString();
	Material* pMaterial = Engine::GetInstance()->GetMaterial(m_MaterialFile.c_str());
	pMaterial->SetEffect(Engine::GetInstance()->GetEffect("Shaders/debug_pbl_instanced.json"));
	m_MaterialKey = pMaterial->GetKey();


	auto& pos = json_value["relative_position"].GetArray();
	m_Orientation.SetPosition({ (float)pos[0].GetDouble(), (float)pos[1].GetDouble(), (float)pos[2].GetDouble(), 1.f });

	auto& scale = json_value["relative_scale"].GetArray();
	m_Scale = { (float)scale[0].GetDouble(), (float)scale[1].GetDouble(), (float)scale[2].GetDouble(), 1.f };

	auto& rot = json_value["relative_rotation"].GetArray();
	m_Rotation = { (float)rot[0].GetDouble(), (float)rot[1].GetDouble(), (float)rot[2].GetDouble(), 1.f };

	m_Shadowed = json_value["shadowed"].GetBool();

}

void GraphicsComponent::Deserialize(const rapidjson::Value& json_value)
{
	for (const rapidjson::Value& value : json_value["instances"].GetArray())
	{
		ModelInstance instance;
		instance.Deserialize(value);
		m_Instances.Add(instance);
	}
}