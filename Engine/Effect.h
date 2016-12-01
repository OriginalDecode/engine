#pragma once
#include "snowblind_shared.h"

namespace Snowblind
{
	struct CompiledShader;	
	class Texture;
	class Effect
	{
		friend class ShaderFactory;
	public: 
		Effect(const std::string& aFilePath);
		~Effect();
		

		CompiledShader* GetVertexShader() { return m_VertexShader; }
		CompiledShader* GetPixelShader() { return m_PixelShader; }

		/* TODO : IMPLEMENT THE FOLLOWING SHADERS */
		CompiledShader* GetGeometryShader();
		CompiledShader* GetHullShader();
		CompiledShader* GetDomainShader();
		CompiledShader* GetComputeShader();

		void Activate();
		void Deactivate();
		void AddShaderResource(IShaderResourceView* aShaderResource);
	private:
		std::string myFileName;
		ID3D11DeviceContext* myContext;

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
};