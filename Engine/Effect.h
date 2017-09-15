#pragma once
#include "engine_shared.h"

struct CompiledShader;
class Texture;
class Effect
{
	friend class ShaderFactory;
public:

	enum TextureSlot
	{
		DIFFUSE,
		ALBEDO = DIFFUSE,
		REFRACTION = DIFFUSE,
		NORMAL,
		LUMINANCE = NORMAL,
		ROUGHNESS,
		AVG_LUMINANCE = ROUGHNESS,
		METALNESS,
		REFLECTION = METALNESS,
		LUMINANCE_COPY = METALNESS,
		EMISSIVE,
		OPACITY,
		PARTICLES = OPACITY,
		AO,
		DEPTH,
		SHADOWMAP,
		CUBEMAP,
		SSAO,
		DUDV,
		_3DTEX,
		_COUNT
	};

	Effect(const std::string& aFilePath);
	Effect() = default;
	CompiledShader* GetVertexShader() { return m_VertexShader; }
	CompiledShader* GetPixelShader() { return m_PixelShader; }
	CompiledShader* GetGeometryShader() { return m_GeometryShader; }
	CompiledShader* GetHullShader() { return m_HullShader; }
	CompiledShader* GetDomainShader() { return m_DomainShader; }
	CompiledShader* GetComputeShader() { return m_ComputeShader; }

	void AddShaderResource(void* pResource, TextureSlot slot);
	void AddShaderResource(Texture* pResource, TextureSlot slot);

	void Use();
	void Clear();

private:
	std::string m_FileName;

	CompiledShader* m_VertexShader		= nullptr;
	CompiledShader* m_PixelShader		= nullptr;
	CompiledShader* m_GeometryShader	= nullptr;
	CompiledShader* m_HullShader		= nullptr;
	CompiledShader* m_DomainShader		= nullptr;
	CompiledShader* m_ComputeShader		= nullptr;


	void* m_Resources[_COUNT];
};

