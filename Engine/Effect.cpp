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

#ifdef SNOWBLIND_DX11
		myContext = CEngine::GetAPI()->GetContext();
#endif
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
#ifdef SNOWBLIND_DX11
		if (myPixelShader != nullptr)
		{
			return myPixelShader;
		}
#endif
		return nullptr;
	}

	void CEffect::Activate()
	{
#ifdef SNOWBLIND_DX11
		CEngine::GetAPI()->SetVertexShader(myVertexShader->vertexShader);
		CEngine::GetAPI()->SetPixelShader(myPixelShader->pixelShader);

		if (myShaderResources.Size() > 0)
		{
			if (!firstOptimize)
			{
				myShaderResources.Optimize();
				myNULLList.Optimize();
				firstOptimize = true;
			}
			CEngine::GetAPI()->GetContext()->PSSetShaderResources(0, myShaderResources.Size(), &myShaderResources[0]);
		}
#endif
	}

	void CEffect::Deactivate()
	{
#ifdef SNOWBLIND_DX11
		CEngine::GetAPI()->GetContext()->PSSetShaderResources(0, myShaderResources.Size(), &myNULLList[0]);
#endif
	}

	void CEffect::AddShaderResource(ID3D11ShaderResourceView* aShaderResource)
	{
#ifdef SNOWBLIND_DX11
		myShaderResources.Add(aShaderResource);
		if (!firstOptimize)
			myNULLList.Add(nullptr);
#endif
	}
};