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

void Material::Use(Effect* pEffect)
{
	Effect* to_use = (pEffect ? pEffect : m_Effect);
	for (const ResourceBinding& binding : m_Resources)
	{
		to_use->AddShaderResource(binding.m_Resource, binding.m_Slot);
	}
	to_use->Use();
}
