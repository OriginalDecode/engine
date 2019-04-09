#include "stdafx.h"
#include "Material.h"
#include <Engine/Texture.h>

Material::Material(uint64 key)
	: m_Key(key)
{
}

void Material::AddResource(IShaderResourceView* pResource, const std::string& filename, TextureSlot slot)
{
	ResourceBinding binding;
	binding.m_Resource = pResource;
	binding.m_Slot = slot;
	binding.m_ResourceName = filename;
	m_Resources.Add(binding);
}

void Material::AddResourceTex(Texture* pResource, const std::string& filename, TextureSlot slot)
{
	AddResource(pResource->GetShaderView(), filename, slot);
}

void Material::Serialize(std::string file_name)
{
	size_t pos = file_name.rfind("/");
	file_name = file_name.substr(pos + 1);
	size_t end = file_name.rfind(".");
	file_name = file_name.substr(0, end);

	std::string str("Data/Material/");
	str += file_name;
	str += ".json";

	std::ofstream output;
	output.open(str.c_str(), std::ios::app);


	if (output.is_open())
	{
		output << "{\n";
		output << "\"material\":{\n";
		output << "\"albedo\":\"";
		output << GetFilename(TextureSlot::ALBEDO).c_str();
		output << "\",\n\"normal\":\"";
		output << GetFilename(TextureSlot::NORMAL).c_str();

		output << "\",\n\"metalness\":\"";
		output << GetFilename(TextureSlot::METALNESS).c_str();

		output << "\",\n\"roughness\":\"";
		output << GetFilename(TextureSlot::ROUGHNESS).c_str();

		output << "\"}\n";
		output << "}\n";

		output.flush();
		output.close();
	}
}

void Material::Use(Effect* pEffect, bool _override)
{
	Effect* _use = (pEffect ? pEffect : m_Effect);
	auto& ctx = Engine::GetAPI()->GetContext();
	for (ResourceBinding& binding : m_Resources)
	{
		_use->AddShaderResource(binding.m_Resource, binding.m_Slot);
	}

	bool prev = _use->set_shaders;
	_use->set_shaders = false;
	_use->Use();

	_use->set_shaders = prev;




}

const CU::GrowingArray<Material::ResourceBinding>& Material::GetResourceBindings() const
{
	return m_Resources;
}

std::string Material::GetFilename(TextureSlot slot)
{
	for (const ResourceBinding& b : m_Resources)
	{
		if (b.m_Slot == slot)
		{
			return b.m_ResourceName;
		}
	}
	return "";
}
