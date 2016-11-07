#include "stdafx.h"
#include "Surface.h"

#include "DirectX11.h"
#include "Engine.h"

#include "Texture.h"
#include "Effect.h"

#include "AssetsContainer.h"

namespace Snowblind
{
	CSurface::CSurface(CEffect* anEffect)
	{
#ifdef SNOWBLIND_DX11
		myContext = CEngine::GetAPI()->GetContext();
#endif
		SetVertexCount(0);
		SetVertexStart(0);
		SetIndexCount(0);
		SetIndexStart(0);
		SetEffect(anEffect);
		SetPrimology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	CSurface::CSurface(CEffect* anEffect, unsigned int aStartVertex, unsigned int aVertexCount, unsigned int aStartIndex, unsigned int aIndexCount)
	{
#ifdef SNOWBLIND_DX11
		myContext = CEngine::GetAPI()->GetContext();
#endif
		SetVertexCount(aVertexCount);
		SetVertexStart(aStartVertex);
		SetIndexCount(aIndexCount);
		SetIndexStart(aStartIndex);
		SetEffect(anEffect);
		SetPrimology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	CSurface::CSurface(unsigned int aStartVertex, unsigned int aVertexCount, unsigned int aStartIndex, unsigned int anIndexCount, D3D_PRIMITIVE_TOPOLOGY aPrimology)
	{
#ifdef SNOWBLIND_DX11
		myContext = CEngine::GetAPI()->GetContext();
#endif
		SetVertexCount(aVertexCount);
		SetVertexStart(aStartVertex);
		SetIndexCount(anIndexCount);
		SetIndexStart(aStartIndex);
		SetPrimology(aPrimology);
	}

	CSurface::~CSurface()
	{
		myTextures.DeleteAll();
		myResourceNames.RemoveAll();
		myFileNames.RemoveAll();
	}

	void CSurface::Activate()
	{
		if (myShaderViews.Size() > 0)
		{
			if (!firstOptimize)
			{
				myShaderViews.Optimize();
				myNullList.Optimize();
				firstOptimize = true;
			}
			myContext->IASetPrimitiveTopology(myPrimologyType);
			myContext->PSSetShaderResources(0, myShaderViews.Size(), &myShaderViews[0]);
		}
	}

	void CSurface::Deactivate()
	{
		if (myNullList.Size() > 0)
		{
			myContext->PSSetShaderResources(0, myNullList.Size(), &myNullList[0]);
		}
	}
	
	void CSurface::AddTexture(const std::string& aResourceName, const std::string& aFilePath)
	{
		if (aResourceName == "AOTexture")
		{
			return;
		}
		myFileNames.Add(aFilePath);
		myResourceNames.Add(aResourceName);
		
		std::string sub = CL::substr(aFilePath, ".png", true, 0);
		std::string debugName = sub;
		if (CL::substr(sub, ".dds") == false)
		{
			sub += ".dds";
		}
		STexture* newTexture = new STexture; //not a memoryleak.
		newTexture->texture = CEngine::GetInstance()->GetTexture(sub);
		newTexture->resourceName = aResourceName;
		myTextures.Add(newTexture);
#ifdef SNOWBLIND_DX11
		myShaderViews.Add(newTexture->texture->GetShaderView());
#endif
		myNullList.Add(nullptr);
	}

	void CSurface::SetEffect(CEffect* anEffect)
	{
		myEffect = anEffect;
	}

	void CSurface::SetVertexStart(unsigned int aStartVertex)
	{
		myVertexStart = aStartVertex;
	}

	void CSurface::SetVertexCount(unsigned int aVertexCount)
	{
		myVertexCount = aVertexCount;
	}

	void CSurface::SetIndexStart(unsigned int aStartIndex)
	{
		myIndexStart = aStartIndex;
	}

	void CSurface::SetIndexCount(unsigned int aIndexCount)
	{
		myIndexCount = aIndexCount;
	}

	void CSurface::SetPrimology(D3D_PRIMITIVE_TOPOLOGY aPrimology)
	{
		myPrimologyType = aPrimology;
	}
};