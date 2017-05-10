#include "stdafx.h"
#include "Surface.h"

#include "DirectX11.h"
#include "Engine.h"

#include "Texture.h"
#include "Effect.h"

#include "AssetsContainer.h"
#include <DL_Debug.h>


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
}

void CSurface::ClearTextures()
{
}

void CSurface::Activate()
{
	if ( myShaderViews.Empty() )
		return;

	myContext->IASetPrimitiveTopology(myPrimologyType);
	myContext->PSSetShaderResources(0, myShaderViews.Size(), &myShaderViews[0]);

}

void CSurface::Deactivate()
{
	if(m_Null.Size() > 0)
		myContext->PSSetShaderResources(0, m_Null.Size(), &m_Null[0]);
}

void CSurface::AddTexture(const std::string& file_path) 
{
	//m_ContainingTextures |= type;

	std::string sub = file_path;
	if (file_path.find(".dds") == file_path.npos)
	{
		DL_WARNING("Incorrect filetype! %s", file_path.c_str());

		size_t pos = file_path.find(".");
		sub = file_path.substr(0, pos);
		sub += ".dds";
	}
	
	//myShaderViews.Add(Engine::GetInstance()->GetTexture(sub)->GetShaderView());








	/**
	STexture new_texture;
	new_texture.m_Type = type;

	if (Engine::GetInstance()->GetTexture(sub))
		new_texture.texture = Engine::GetInstance()->GetTexture(sub)->GetShaderView();
	else
	{
		DL_MESSAGE("Failed to load %s", sub.c_str());
		new_texture.texture = Engine::GetInstance()->GetTexture("Data/Textures/default_textures/no-texture-bw.dds")->GetShaderView();
	}


	myTextures.Add(new_texture);

	myFileNames.Add(sub);


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

	/**/
}

void CSurface::AddTexture(IShaderResourceView* texture)
{
	myShaderViews.Add(texture);
	m_Null.Add(nullptr);
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

void Material::AddResource(IShaderResourceView* pResource, Effect::TextureSlot slot)
{
	ResourceBinding binding;
	binding.m_Resource = pResource;
	binding.m_Slot = slot;
}

void Material::AddResource(Texture* pResource, Effect::TextureSlot slot)
{
	AddResource(pResource->GetShaderView(), slot);
}

void Material::Use(Effect* pEffect, const RenderContext& render_context)
{
	for (const ResourceBinding& binding : m_Resources)
	{
		pEffect->AddShaderResource(binding.m_Resource, binding.m_Slot);
	}
	pEffect->Use();
}

void Material::Clear()
{

}
