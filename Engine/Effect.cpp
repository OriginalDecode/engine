#include "stdafx.h"
#include "Effect.h"
namespace Snowblind
{
	Effect::Effect(const std::string& aFilePath)
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
		myContext = Engine::GetAPI()->GetContext();
#endif
	}

	Effect::~Effect()
	{
		/*SAFE_DELETE(myVertexShader);
		SAFE_DELETE(myPixelShader);
		SAFE_DELETE(myGeometryShader);
		SAFE_DELETE(myHullShader);
		SAFE_DELETE(myDomainShader);
		SAFE_DELETE(myComputeShader);*/
	}

	VertexShader* Effect::GetVertexShader()
	{
		return myVertexShader;
	}

	PixelShader* Effect::GetPixelShader()
	{
#ifdef SNOWBLIND_DX11
		if (myPixelShader != nullptr)
		{
			return myPixelShader;
		}
#endif
		return nullptr;
	}

	void Effect::Activate()
	{
#ifdef SNOWBLIND_DX11
		Engine::GetAPI()->SetVertexShader(myVertexShader->vertexShader);
		Engine::GetAPI()->SetPixelShader(myPixelShader->pixelShader);

		if (myShaderResources.Size() > 0)
		{
			myShaderResources.Optimize();
			Engine::GetAPI()->GetContext()->PSSetShaderResources(0, myShaderResources.Size(), &myShaderResources[0]);
		}
#endif
	}

	void Effect::Deactivate()
	{
#ifdef SNOWBLIND_DX11
		Engine::GetAPI()->GetContext()->PSSetShaderResources(0, myShaderResources.Size(), &myNULLList[0]);
#endif
	}

	void Effect::AddShaderResource(ID3D11ShaderResourceView* aShaderResource)
	{
#ifdef SNOWBLIND_DX11
		myShaderResources.Add(aShaderResource);
		if (!firstOptimize)
			myNULLList.Add(nullptr);
#endif
	}
};