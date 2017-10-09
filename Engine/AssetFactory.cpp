#include "stdafx.h"
#include "AssetFactory.h"
#include <Engine/ModelImporter.h>
#include <Engine/Engine.h>
#include <Engine/Material.h>
AssetFactory::AssetFactory()
{
	m_ModelImporter = new CModelImporter;
}


void AssetFactory::CreateMaterial(const std::string& filepath, Material* pMaterial)
{

	Engine* pEngine = Engine::GetInstance();
	JSONReader reader;
	reader.OpenDocument(filepath);


	const JSONElement& el = reader.GetElement("material");
	
	if (reader.ElementHasMember(el, "albedo"))
	{
		std::string albedo = el["albedo"].GetString();
		pMaterial->AddResource(pEngine->GetTexture(albedo.c_str()), albedo, Effect::DIFFUSE);
	}

	if (reader.ElementHasMember(el, "normal"))
	{
		std::string normal = el["normal"].GetString();
		pMaterial->AddResource(pEngine->GetTexture(normal.c_str()), normal, Effect::NORMAL);
	}

	if (reader.ElementHasMember(el, "metalness"))
	{
		std::string metalness = el["metalness"].GetString();
		pMaterial->AddResource(pEngine->GetTexture(metalness.c_str()), metalness, Effect::METALNESS);
	}

	if (reader.ElementHasMember(el, "roughness"))
	{
		std::string roughness = el["roughness"].GetString();
		pMaterial->AddResource(pEngine->GetTexture(roughness.c_str()), roughness, Effect::ROUGHNESS);
	}

	if (reader.ElementHasMember(el, "emissive"))
	{
		std::string emissive = el["emissive"].GetString();
		pMaterial->AddResource(pEngine->GetTexture(emissive.c_str()), emissive, Effect::EMISSIVE);
	}


}
