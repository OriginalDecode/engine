#include "stdafx.h"
#include "Effect.h"
#include <d3dcompiler.h>
#include "ShaderContainer.h"
namespace Snowblind
{
	CEffect::CEffect(const std::string& aFilePath)
		: myFileName(aFilePath)
	{
		myVertexShader = new SVertexShader();
		myPixelShader = new SPixelShader();
		myGeometryShader = new SGeometryShader();
		myHullShader = new SHullShader();
		myDomainShader = new SDomainShader();
		myComputeShader = new SComputeShader();
		
		myContext = CEngine::GetDirectX()->GetContext();
	}

	CEffect::~CEffect()
	{
		SAFE_DELETE(myVertexShader);
		SAFE_DELETE(myPixelShader);
		SAFE_DELETE(myGeometryShader);
		SAFE_DELETE(myHullShader);
		SAFE_DELETE(myDomainShader);
		SAFE_DELETE(myComputeShader);
	}

	SVertexShader* CEffect::GetVertexShader()
	{
		return myVertexShader;
	}

	SPixelShader* CEffect::GetPixelShader()
	{
		return myPixelShader;
	}

}