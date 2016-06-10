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
		SetVertexCount(0);
		SetVertexStart(0);
		SetIndexCount(0);
		SetIndexStart(0);
		SetEffect(anEffect);
		SetPrimology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	CSurface::CSurface(CEffect* anEffect, unsigned int aStartVertex, unsigned int aVertexCount, unsigned int aStartIndex, unsigned int aIndexCount)
	{
		SetVertexCount(aVertexCount);
		SetVertexStart(aStartVertex);
		SetIndexCount(aIndexCount);
		SetIndexStart(aStartIndex);
		SetEffect(anEffect);
		SetPrimology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	CSurface::CSurface(unsigned int aStartVertex, unsigned int aVertexCount, unsigned int aStartIndex, unsigned int anIndexCount, D3D_PRIMITIVE_TOPOLOGY aPrimology)
	{
		SetVertexCount(aVertexCount);
		SetVertexStart(aStartVertex);
		SetIndexCount(anIndexCount);
		SetIndexStart(aStartIndex);
		SetPrimology(aPrimology);
	}

	CSurface::~CSurface()
	{
		myShaderVariables.RemoveAll();
		myTextures.DeleteAll();
		myResourceNames.RemoveAll();
		myFileNames.RemoveAll();
	}

	void CSurface::Activate()
	{
		CEngine::GetDirectX()->GetContext()->IASetPrimitiveTopology(myPrimologyType);
		for (int i = 0; i < myTextures.Size(); ++i)
		{
			//myEffect->SetTexture(myTextures[i]->texture->GetShaderView(), myTextures[i]->resourceName);
			//myShaderVariables[i]->SetResource(myTextures[i]->GetShaderView());
		}
	}

	void CSurface::SetTexture(const std::wstring& aResourceName, const std::wstring& aFilePath)
	{
		if (aResourceName == L"AOTexture")
		{
			return;
		}
		myFileNames.Add(aFilePath);
		myResourceNames.Add(aResourceName);


		/*ID3DX11EffectShaderResourceVariable* srv;
		myEffect->GetShaderResource(srv, aResourceName);*/
		//myShaderVariables.Add(srv);

		STexture* newTexture = new STexture();
		newTexture->texture = CAssetsContainer::GetInstance()->GetTexture(aFilePath);
		newTexture->resourceName = aResourceName;
		myTextures.Add(newTexture);
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