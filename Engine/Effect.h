#pragma once
#include "engine_shared.h"


struct CompiledShader;
class Texture;

class Effect
{
	friend class ShaderFactory;
public:
	Effect(const std::string& aFilePath);
	Effect() = default;
	CompiledShader* GetVertexShader() { return m_VertexShader; }
	CompiledShader* GetPixelShader() { return m_PixelShader; }
	CompiledShader* GetGeometryShader() { return m_GeometryShader; }
	CompiledShader* GetHullShader() { return m_HullShader; }
	CompiledShader* GetDomainShader() { return m_DomainShader; }
	CompiledShader* GetComputeShader() { return m_ComputeShader; }

	void SetPixelShader(CompiledShader* shader);

	void AddShaderResource(IShaderResourceView* aShaderResource);

	enum TextureSlot
	{
		DIFFUSE,
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
		_COUNT
	};


	void AddShaderResource(IShaderResourceView* pResource, TextureSlot slot);
	void AddShaderResource(Texture* pResource, TextureSlot slot);
	void Use();
	void Clear();


private:
	std::string myFileName;

	CompiledShader* m_VertexShader		= nullptr;
	CompiledShader* m_PixelShader		= nullptr;
	CompiledShader* m_GeometryShader	= nullptr;
	CompiledShader* m_HullShader		= nullptr;
	CompiledShader* m_DomainShader		= nullptr;
	CompiledShader* m_ComputeShader		= nullptr;

	IShaderResourceView* m_Resources[_COUNT];

	CU::GrowingArray<IShaderResourceView*> myShaderResources;
	CU::GrowingArray<IShaderResourceView*> myNULLList;
	bool firstOptimize = false;
};

