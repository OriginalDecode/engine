#pragma once
#include <Engine/engine_shared.h>
#include <Engine/IGraphicsAPI.h>
#include <DataStructures/GrowingArray.h>
#include <string>
#include <Engine/Effect.h>

class Texture;
class Material
{
public:
	Material() = default;
	Material(u64 key);
	void SetEffect(Effect* effect) { m_Effect = effect; }
	void AddResource(IShaderResourceView* pResource, const std::string& filename, Effect::TextureSlot slot);
	void AddResource(Texture* pResource, const std::string& filename, Effect::TextureSlot slot);

	u64 GetKey() const { return m_Key; }

	void Use(Effect* pEffect);
private:
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

