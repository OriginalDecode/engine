#include "stdafx.h"
#include "Surface.h"

#include <d3dx11effect.h>

#include "DirectX11.h"
#include "Engine.h"

#include "Texture.h"
#include "TextureContainer.h"

#include "Effect.h"
#include "EffectContainer.h"

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
		myTextures.RemoveAll();
		myResourceNames.RemoveAll();
		myFileNames.RemoveAll();
	}

	void CSurface::Activate()
	{
		CEngine::GetInstance()->GetAPI()->GetContext()->IASetPrimitiveTopology(myPrimologyType);
		for (int i = 0; i < myShaderVariables.Size(); ++i)
		{
			myShaderVariables[i]->SetResource(myTextures[i]->GetShaderView());
		}
	}

	void CSurface::SetTexture(const std::string& aResourceName, const std::string& aFilePath)
	{
		if (aResourceName == "AOTexture")
		{
			return;
		}
		myFileNames.Add(aFilePath);
		myResourceNames.Add(aResourceName);

		CTexture* tempTexture = CTextureContainer::GetInstance()->GetTexture(aFilePath);
		DL_ASSERT_EXP(tempTexture != nullptr, "[Surface](SetTexture) : Failed to set Texture!");

		ID3DX11EffectShaderResourceVariable* tempShader = myEffect->GetEffect()->GetVariableByName(aResourceName.c_str())->AsShaderResource();
		myEffect->Validate(tempShader, "Effect invalid!");
		DL_ASSERT_EXP(tempShader != nullptr, "[Surface](SetTexture) : Failed to set Shader!");
		myTexture = tempTexture;
		myShaderVariables.Add(tempShader);
		myTextures.Add(tempTexture);
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