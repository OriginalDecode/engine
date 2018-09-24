#pragma once

#include <Engine/engine_shared.h>
#include <Engine/Effect.h>

#include <DataStructures/GrowingArray.h>

#include <string>

class Texture;



struct MaterialData //Should be loaded from external file? Reload if needed?
{
	enum MaterialTypes
	{
		DEFAULT,
		NOF_TYPES
	};

	float x, y, z, w;

};

class Material
{
public:
	Material() = default;
	Material(u64 key); // This key should be created when we create the material and never changed unless the version changes, which should happen only when we introduce new data and such.
	void SetEffect(Effect* effect) { m_Effect = effect; }
	void AddResource(IShaderResourceView* pResource, const std::string& filename, Effect::TextureSlot slot);
	void AddResource(Texture* pResource, const std::string& filename, Effect::TextureSlot slot);

	u64 GetKey() const { return m_Key; }


	void Serialize(std::string file_name);

	void Use(Effect* pEffect, bool _override = false);
private:
	MaterialData* m_Data = nullptr;

	std::string GetFilename(Effect::TextureSlot slot);

	u64 m_Key = 0;
	Effect* m_Effect = nullptr;

	struct ResourceBinding
	{
		IShaderResourceView* m_Resource;
		Effect::TextureSlot m_Slot;
		std::string m_ResourceName;

	};

	CU::GrowingArray<ResourceBinding> m_Resources;
};

