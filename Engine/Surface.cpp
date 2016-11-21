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
		myFileNames.RemoveAll();

		//Can cause heap corruption?
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

	void CSurface::AddTexture(const std::string& file_path, TextureType type)
	{
		m_ContainingTextures |= type;

		myFileNames.Add(file_path);
		std::string sub = CL::substr(file_path, ".png", true, 0);
		std::string debugName = sub;
		if (CL::substr(sub, ".dds") == false)
			sub += ".dds";

		STexture* new_texture = new STexture; 
		new_texture->texture = CEngine::GetInstance()->GetTexture(sub);
		new_texture->m_Type = type;
		myTextures.Add(new_texture);

		std::sort(myTextures.begin(), myTextures.end(), [&](STexture* first, STexture* second) {
			return first->m_Type < second->m_Type;
		});


#ifdef SNOWBLIND_DX11
		myShaderViews.Add(new_texture->texture->GetShaderView());
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

	void CSurface::AddMissingTexture(TextureType type)
	{
		if (!(m_ContainingTextures & type))
		{
			AddTexture("Data/Textures/No-Texture.dds", type);
			return;
		}
		MODEL_LOG("Already contained texture");
	}

	void CSurface::ValidateTextures()
	{
		MODEL_LOG("Validating Textures of surface.");

		AddMissingTexture(_ALBEDO);
		AddMissingTexture(_NORMAL);
		AddMissingTexture(_ROUGHNESS);
		AddMissingTexture(_METALNESS);
		AddMissingTexture(_EMISSIVE);
		AddMissingTexture(_AO);
	}

	
};