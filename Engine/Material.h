#pragma once

#include <Engine/engine_shared.h>
#include <Engine/Effect.h>

#include <DataStructures/GrowingArray.h>

#include <string>

class Texture;

class Material
{
	friend class ModelExporter;
public:
	struct ResourceBinding
	{
		IShaderResourceView* m_Resource;
		TextureSlot m_Slot;
		std::string m_ResourceName;
	};

	Material() = default;
	Material(uint64 key); // This key should be created when we create the material and never changed unless the version changes, which should happen only when we introduce new data and such.
	void SetEffect(Effect* effect) { m_Effect = effect; }
	void AddResource(IShaderResourceView* pResource, const std::string& filename, TextureSlot slot);
	void AddResourceTex(Texture* pResource, const std::string& filename, TextureSlot slot);

	uint64 GetKey() const { return m_Key; }


	void Serialize(std::string file_name);

	void Use(Effect* pEffect, bool _override = false);

	const CU::GrowingArray<ResourceBinding>& GetResourceBindings() const;

private:
	std::string GetFilename(TextureSlot slot);

	uint64 m_Key = 0;
	Effect* m_Effect = nullptr;

	

	CU::GrowingArray<ResourceBinding> m_Resources;
};