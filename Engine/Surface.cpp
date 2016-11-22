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
			myShaderViews.Optimize();
			myContext->IASetPrimitiveTopology(myPrimologyType);
			myContext->PSSetShaderResources(0, myShaderViews.Size(), &myShaderViews[0]);
		}
	}

	void CSurface::Deactivate()
	{
		//ID3D11ShaderResourceView* view[1] = { nullptr };
		//myContext->PSSetShaderResources(0, myShaderViews.Size(), view);
	}

	std::string CheckTextureType(TextureType type)
	{
		switch (type)
		{
			case _ALBEDO: return "ALBEDO";
			case _NORMAL: return "NORMAL";
			case _METALNESS: return "METALNESS";
			case _ROUGHNESS: return "ROUGHNESS";
			case _AO: return "AO";
			case _EMISSIVE: return "EMISSIVE";
		}
		return "failed";
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
			DL_MESSAGE("\nFirst : %d\nType : %s", (u64)first, CheckTextureType(first->m_Type).c_str());
			DL_MESSAGE("\nSecond : %d\nType : %s", (u64)second, CheckTextureType(second->m_Type).c_str());
			return first->m_Type < second->m_Type;
		});

		for (s32 i = 0; i < myTextures.Size(); i++)
		{
			DL_MESSAGE("\nIndex : %d\nPointer : %d\nType : %s", i, (u64)myTextures[i], CheckTextureType(myTextures[i]->m_Type).c_str());
		}



#ifdef SNOWBLIND_DX11
		myShaderViews.Add(new_texture->texture->GetShaderView());
#endif
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