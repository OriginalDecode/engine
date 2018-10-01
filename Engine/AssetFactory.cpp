#include "stdafx.h"
#include "AssetFactory.h"
#include <Engine/ModelImporter.h>
#include <Engine/Engine.h>
#include <Engine/Material.h>

AssetFactory* AssetFactory::m_Instance = nullptr;

AssetFactory::AssetFactory()
{
}


void AssetFactory::Create()
{
	m_Instance = new AssetFactory;
}

void AssetFactory::Destroy()
{
	delete m_Instance;
	m_Instance = nullptr;
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
		pMaterial->AddResource(pEngine->GetTexture(albedo.c_str()), albedo, TextureSlot::DIFFUSE);
	}

	if (reader.ElementHasMember(el, "normal"))
	{
		std::string normal = el["normal"].GetString();
		pMaterial->AddResource(pEngine->GetTexture(normal.c_str()), normal, TextureSlot::NORMAL);
	}

	if (reader.ElementHasMember(el, "metalness"))
	{
		std::string metalness = el["metalness"].GetString();
		pMaterial->AddResource(pEngine->GetTexture(metalness.c_str()), metalness, TextureSlot::METALNESS);
	}

	if (reader.ElementHasMember(el, "roughness"))
	{
		std::string roughness = el["roughness"].GetString();
		pMaterial->AddResource(pEngine->GetTexture(roughness.c_str()), roughness, TextureSlot::ROUGHNESS);
	}

	if (reader.ElementHasMember(el, "emissive"))
	{
		std::string emissive = el["emissive"].GetString();
		pMaterial->AddResource(pEngine->GetTexture(emissive.c_str()), emissive, TextureSlot::EMISSIVE);
	}

	if (reader.ElementHasMember(el, "heightmap"))
	{
		std::string heightmap = el["heightmap"].GetString();
		pMaterial->AddResource(pEngine->GetTexture(heightmap.c_str()), heightmap, TextureSlot::DEPTH);
	}


}
