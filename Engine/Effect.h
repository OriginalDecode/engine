#pragma once
#include "snowblind_shared.h"


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

	void Activate();
	void Deactivate();
	void AddShaderResource(IShaderResourceView* aShaderResource);
private:
	std::string myFileName;
	IDevContext* m_Context;

	CompiledShader* m_VertexShader		= nullptr;
	CompiledShader* m_PixelShader		= nullptr;
	CompiledShader* m_GeometryShader	= nullptr;
	CompiledShader* m_HullShader		= nullptr;
	CompiledShader* m_DomainShader		= nullptr;
	CompiledShader* m_ComputeShader		= nullptr;

	CU::GrowingArray<IShaderResourceView*> myShaderResources;
	CU::GrowingArray<IShaderResourceView*> myNULLList;
	bool firstOptimize = false;
};
