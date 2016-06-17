#pragma once
#include <string>
#include "LightStructs.h"
#include <DataStructures/StaticArray.h>
namespace Snowblind
{

	struct SVertexShader;
	struct SPixelShader;
	struct SGeometryShader;
	struct SHullShader;
	struct SDomainShader;
	struct SComputeShader;

	class CTexture;
	class CEffect
	{
		friend class CShaderFactory;
	public:
		CEffect(const std::string& aFilePath);
		~CEffect();
		void Activate();


	private:
		std::string myFileName;

		SVertexShader* myVertexShader = nullptr;
		SPixelShader* myPixelShader = nullptr;
		SGeometryShader* myGeometryShader = nullptr;
		SHullShader* myHullShader = nullptr;
		SDomainShader* myDomainShader = nullptr;
		SComputeShader* myComputeShader = nullptr;

	};
};