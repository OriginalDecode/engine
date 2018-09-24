#include "stdafx.h"
#include "Material.h"

Material::Material(u64 key)
	: m_Key(key)
{
}

void Material::AddResource(IShaderResourceView* pResource, const std::string& filename, Effect::TextureSlot slot)
{
	ResourceBinding binding;
	binding.m_Resource = pResource;
	binding.m_Slot = slot;
	binding.m_ResourceName = filename;
	m_Resources.Add(binding);
}

void Material::AddResource(Texture* pResource, const std::string& filename, Effect::TextureSlot slot)
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
		output << GetFilename(Effect::ALBEDO).c_str();
		output << "\",\n\"normal\":\"";
		output << GetFilename(Effect::NORMAL).c_str();

		output << "\",\n\"metalness\":\"";
		output << GetFilename(Effect::METALNESS).c_str();

		output << "\",\n\"roughness\":\"";
		output << GetFilename(Effect::ROUGHNESS).c_str();

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
	for (const ResourceBinding& binding : m_Resources)
	{

		if (_override)
		{
			IShaderResourceView* view = binding.m_Resource;
			ctx.PSSetShaderResource(binding.m_Slot, 1, &view);
		}
		else
		{
			_use->AddShaderResource(binding.m_Resource, binding.m_Slot);
		}
	}
	_use->Use();
}

std::string Material::GetFilename(Effect::TextureSlot slot)
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
