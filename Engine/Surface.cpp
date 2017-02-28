#include "stdafx.h"
#include "Surface.h"

#include "DirectX11.h"
#include "Engine.h"

#include "Texture.h"
#include "Effect.h"

#include "AssetsContainer.h"
#include <DL_Debug.h>

namespace Hex
{
	CSurface::CSurface(Effect* anEffect)
	{
		myContext = Engine::GetAPI()->GetContext();
		SetVertexCount(0);
		SetVertexStart(0);
		SetIndexCount(0);
		SetIndexStart(0);
		SetEffect(anEffect);
		SetPrimology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	CSurface::CSurface(Effect* anEffect, u32 aStartVertex, u32 aVertexCount, u32 aStartIndex, u32 aIndexCount)
	{
		myContext = Engine::GetAPI()->GetContext();
		SetVertexCount(aVertexCount);
		SetVertexStart(aStartVertex);
		SetIndexCount(aIndexCount);
		SetIndexStart(aStartIndex);
		SetEffect(anEffect);
		SetPrimology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	CSurface::CSurface(u32 aStartVertex, u32 aVertexCount, u32 aStartIndex, u32 anIndexCount, D3D_PRIMITIVE_TOPOLOGY aPrimology)
	{
		myContext = Engine::GetAPI()->GetContext();
		SetVertexCount(aVertexCount);
		SetVertexStart(aStartVertex);
		SetIndexCount(anIndexCount);
		SetIndexStart(aStartIndex);
		SetPrimology(aPrimology);
	}

	CSurface::~CSurface()
	{
		myFileNames.RemoveAll();
		//Can cause heap corruption?
	}

	void CSurface::Activate()
	{
		if (!myShaderViews.Empty())
		{
			myShaderViews.Optimize();
			myContext->IASetPrimitiveTopology(myPrimologyType);
			myContext->PSSetShaderResources(0, myShaderViews.Size(), &myShaderViews[0]);
		}
	}

	void CSurface::Deactivate()
	{
		if(m_Null.Size() > 0)
			myContext->PSSetShaderResources(0, m_Null.Size(), &m_Null[0]);
	}

	std::string CheckTextureType(TextureType type)
	{
		switch (type)
		{
			case _ALBEDO: 
				return "ALBEDO";
			case _NORMAL: 
				return "NORMAL";
			case _METALNESS: 
				return "METALNESS";
			case _ROUGHNESS: 
				return "ROUGHNESS";
			case _AO: 
				return "AO";
			case _EMISSIVE: 
				return "EMISSIVE";
			case _DISPLACEMENT:
				return "DISPLACEMENT";
			case _HEIGHT:
				return "HEIGHT";
			case _LIGHTMAP:
				return "LIGHTMAP";
			case _OPACITY:
				return "OPACITY";
			case _SHININESS:
				return "SHININESS";
			default:return"NO_STRING";
		}
		return "failed";
	}

	void CSurface::AddTexture(const std::string& file_path, TextureType type)
	{
		m_ContainingTextures |= type;

		std::string sub = file_path;
		if (!CL::substr(file_path, ".dds"))
		{
			sub = CL::substr(file_path, ".", true, 0);
			DL_WARNING("Incorrect filetype! %s", file_path.c_str());
		}

		std::string debugName = sub;
		if (CL::substr(sub, ".dds") == false)
			sub += ".dds";

		STexture new_texture;
		new_texture.m_Type = type;

		if(Engine::GetInstance()->GetTexture(sub))
			new_texture.texture = Engine::GetInstance()->GetTexture(sub)->GetShaderView();
		else
			new_texture.texture = Engine::GetInstance()->GetTexture("Data/Textures/default_textures/no-texture-bw.dds")->GetShaderView();


		myTextures.Add(new_texture);

		myFileNames.Add(sub);

		//myTextures.Add(new_texture);

		std::sort(myTextures.begin(), myTextures.end(), [&](STexture& first, STexture& second) {
			return first.m_Type < second.m_Type;
		});

		myShaderViews.RemoveAll();
		m_Null.RemoveAll();
		for (s32 i = 0; i < myTextures.Size(); i++)
		{
			myShaderViews.Add(myTextures[i].texture);
			m_Null.Add(nullptr);
		}
	}

	void CSurface::SetEffect(Effect* anEffect)
	{
		myEffect = anEffect;
	}

	void CSurface::SetVertexStart(u32 aStartVertex)
	{
		myVertexStart = aStartVertex;
	}

	void CSurface::SetVertexCount(u32 aVertexCount)
	{
		myVertexCount = aVertexCount;
	}

	void CSurface::SetIndexStart(u32 aStartIndex)
	{
		myIndexStart = aStartIndex;
	}

	void CSurface::SetIndexCount(u32 aIndexCount)
	{
		myIndexCount = aIndexCount;
	}

	void CSurface::SetPrimology(D3D_PRIMITIVE_TOPOLOGY aPrimology)
	{
		myPrimologyType = aPrimology;
	}

	void CSurface::AddMissingTexture(TextureType type, const std::string& file_path )
	{
		if (!(m_ContainingTextures & type))
		{
			AddTexture(file_path, type);
			return;
		}
		//MODEL_LOG("Already contained texture");
	}

	std::string s_file_path = "Data/Textures/default_textures/";

	void CSurface::ValidateTextures()
	{
		MODEL_LOG("Validating Textures of surface.");

		AddMissingTexture(_ALBEDO,			s_file_path + "white.dds");
		AddMissingTexture(_NORMAL,			s_file_path + "black.dds");
		AddMissingTexture(_ROUGHNESS,		s_file_path + "no-roughness.dds");
		AddMissingTexture(_METALNESS,		s_file_path + "no-metalness.dds");
		AddMissingTexture(_EMISSIVE,		s_file_path + "black.dds");
		AddMissingTexture(_OPACITY,			s_file_path + "black.dds");
		AddMissingTexture(_AO,				s_file_path + "black.dds");
		AddMissingTexture(_HEIGHT,			s_file_path + "black.dds");
		AddMissingTexture(_DISPLACEMENT,	s_file_path + "black.dds");
		AddMissingTexture(_LIGHTMAP,		s_file_path + "black.dds");
		AddMissingTexture(_SHININESS,		s_file_path + "black.dds");
/*
		for (s32 i = 0; i < myTextures.Size(); i++)
		{
			TRACE_LOG("Index : %d | Pointer : %d | Type : %s", i, (u64)myTextures[i].texture, CheckTextureType(myTextures[i].m_Type).c_str());
		}
*/
	}
	
	void CSurface::RemoveTextureByIndex(s32 index)
	{
		myShaderViews.RemoveCyclicAtIndex(index);
	}

};