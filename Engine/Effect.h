#pragma once
#include "engine_shared.h"
#include "ShaderFactory.h"

struct CompiledShader;
class Texture;


enum TextureSlot
{
	REGISTER_0,
	REGISTER_1,
	REGISTER_2,
	REGISTER_3,
	REGISTER_4,
	REGISTER_5,
	REGISTER_6,
	REGISTER_7,
	REGISTER_8,
	REGISTER_9,
	REGISTER_10,
	REGISTER_11,
	REGISTER_12,

	DIFFUSE = REGISTER_0,
	ALBEDO = REGISTER_0,
	REFRACTION = REGISTER_0,

	NORMAL = REGISTER_1,
	LUMINANCE = REGISTER_1,

	ROUGHNESS = REGISTER_2,
	AVG_LUMINANCE = REGISTER_2,

	METALNESS = REGISTER_3,
	REFLECTION = REGISTER_3,
	LUMINANCE_COPY = REGISTER_3,

	EMISSIVE = REGISTER_4,

	OPACITY = REGISTER_5,
	PARTICLES = REGISTER_5,

	AO = REGISTER_6,

	DEPTH = REGISTER_7,

	SHADOWMAP = REGISTER_8,

	CUBEMAP = REGISTER_9,

	SSAO = REGISTER_10,

	DUDV = REGISTER_11,
	SSR = REGISTER_12,

	SLOT_COUNT
};

class Effect : public ShaderReload
{
	friend class ShaderFactory;
public:



	Effect(const std::string& filepath);
	Effect();

	CompiledShader* GetVertexShader() { return m_Shaders[EShaderType_VERTEX]; }
	CompiledShader* GetPixelShader() { return m_Shaders[EShaderType_PIXEL]; }
	CompiledShader* GetGeometryShader() { return m_Shaders[EShaderType_GEOMETRY]; }
	CompiledShader* GetHullShader() { return m_Shaders[EShaderType_HULL]; }
	CompiledShader* GetDomainShader() { return m_Shaders[EShaderType_DOMAIN]; }
	CompiledShader* GetComputeShader() { return m_Shaders[EShaderType_COMPUTE]; }

	void AddShaderResource(IShaderResourceView* pResource, int32 slot);
	void AddShaderResource(Texture* pResource, int32 slot);

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

