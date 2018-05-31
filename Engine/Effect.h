#pragma once
#include "engine_shared.h"

struct CompiledShader;
class Texture;
class Effect
{
	friend class ShaderFactory;
public:

	Effect(const std::string& filepath);
	Effect() = default;
	CompiledShader* GetVertexShader() { return m_VertexShader; }
	CompiledShader* GetPixelShader() { return m_PixelShader; }
	CompiledShader* GetGeometryShader() { return m_GeometryShader; }
	CompiledShader* GetHullShader() { return m_HullShader; }
	CompiledShader* GetDomainShader() { return m_DomainShader; }
	CompiledShader* GetComputeShader() { return m_ComputeShader; }

	void AddShaderResource(IShaderResourceView* pResource, s32 slot);
	void AddShaderResource(Texture* pResource, s32 slot);

	void Use();
	void Clear();
#ifdef _DEBUG
	const std::string& GetFileName() const { return m_FileName; }
#endif
private:
	std::string m_FileName;

	CompiledShader* m_VertexShader		= nullptr;
	CompiledShader* m_PixelShader		= nullptr;
	CompiledShader* m_GeometryShader	= nullptr;
	CompiledShader* m_HullShader		= nullptr;
	CompiledShader* m_DomainShader		= nullptr;
	CompiledShader* m_ComputeShader		= nullptr;



public:


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
		_COUNT
	};

private:
	IShaderResourceView* m_Resources[_COUNT];
	char m_UsedReg[_COUNT];


};

