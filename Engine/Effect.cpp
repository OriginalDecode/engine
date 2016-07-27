#include "stdafx.h"
#include "Effect.h"
namespace Snowblind
{
	CEffect::CEffect(const std::string& aFilePath)
		: myFileName(aFilePath)
	{
		//-----------------------------
		//	Should probably just new the effects that are going to be used.
		//-----------------------------
		/*myVertexShader = new SVertexShader();
		myPixelShader = new SPixelShader();
		myGeometryShader = new SGeometryShader();
		myHullShader = new SHullShader();
		myDomainShader = new SDomainShader();
		myComputeShader = new SComputeShader();*/

		myContext = CEngine::GetDirectX()->GetContext();
	}

	CEffect::~CEffect()
	{
		/*SAFE_DELETE(myVertexShader);
		SAFE_DELETE(myPixelShader);
		SAFE_DELETE(myGeometryShader);
		SAFE_DELETE(myHullShader);
		SAFE_DELETE(myDomainShader);
		SAFE_DELETE(myComputeShader);*/
	}

	SVertexShader* CEffect::GetVertexShader()
	{
		return myVertexShader;
	}

	SPixelShader* CEffect::GetPixelShader()
	{
		if (myPixelShader != nullptr)
		{
			return myPixelShader;
		}
		return nullptr;
	}

	void CEffect::ActivateShaderResources()
	{
		if (!firstOptimize)
		{
			myShaderResources.Optimize();
			firstOptimize = true;
		}
		CEngine::GetDirectX()->GetContext()->PSSetShaderResources(0, myShaderResources.Size(), &myShaderResources[0]);
	}

	void CEffect::DeactivateShaderResources()
	{
		CEngine::GetDirectX()->GetContext()->PSSetShaderResources(0, myShaderResources.Size(), &myNULLList[0]);
		myShaderResources.RemoveAll();
	}

	void CEffect::AddShaderResource(ID3D11ShaderResourceView* aShaderResource)
	{
		myShaderResources.Add(aShaderResource);
		if (!firstOptimize)
			myNULLList.Add(nullptr);
	}

}