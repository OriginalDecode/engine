#pragma once
#include "engine_shared.h"
#include "ShaderFactory.h"

struct CompiledShader;
class Texture;


#define TEXTURE_SLOT(x) x,
#define TEXTURE_SLOT_ALT(x) x
enum TextureSlot
{
#include "TextureSlotEnum.h"
	SLOT_COUNT
};
#undef TEXTURE_SLOT
#undef TEXTURE_SLOT_ALT

#define TEXTURE_SLOT_ALT(x) #x,
static const char* texture_slots[] = {
#include "TextureSlotEnum.h"
};
#undef TEXTURE_SLOT_ALT

class Effect : public ShaderReload
{
	friend class ShaderFactory;
public:

	Effect(const std::string& filepath);
	Effect();

	CompiledShader* GetVertexShader() { return m_Shaders[VERTEX]; }
	CompiledShader* GetPixelShader() { return m_Shaders[PIXEL]; }
	CompiledShader* GetGeometryShader() { return m_Shaders[GEOMETRY]; }
	CompiledShader* GetHullShader() { return m_Shaders[HULL]; }
	CompiledShader* GetDomainShader() { return m_Shaders[DOMAINS]; }
	CompiledShader* GetComputeShader() { return m_Shaders[COMPUTE]; }

	void AddShaderResource(IShaderResourceView* pResource, s32 slot);
	void AddShaderResource(Texture* pResource, s32 slot);

	bool set_shaders = true;

	void Use();
	void Clear();

	void Reload(CompiledShader* shader) override;


#ifdef _DEBUG
	const std::string& GetFileName() const { return m_FileName; }
#endif
private:
	std::string m_FileName;

	CompiledShader* m_Shaders[eShaderType::NOF_TYPES];

public:



private:
	IShaderResourceView* m_Resources[SLOT_COUNT];
	char m_UsedReg[SLOT_COUNT];


};

