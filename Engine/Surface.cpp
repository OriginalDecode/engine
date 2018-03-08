#include "stdafx.h"
#include "Surface.h"

#include "Engine.h"

#include "Texture.h"
#include "Effect.h"

#include "AssetsContainer.h"
#include <DL_Debug.h>
#include <Engine/IGraphicsContext.h>


Surface::Surface(Effect* anEffect)
{
	SetVertexCount(0);
	SetVertexStart(0);
	SetIndexCount(0);
	SetIndexStart(0);
	SetEffect(anEffect);
	//SetPrimology(graphics::TRIANGLE_LIST);
}

Surface::Surface(Effect* anEffect, u32 aStartVertex, u32 aVertexCount, u32 aStartIndex, u32 aIndexCount)
{
	SetVertexCount(aVertexCount);
	SetVertexStart(aStartVertex);
	SetIndexCount(aIndexCount);
	SetIndexStart(aStartIndex);
	SetEffect(anEffect);
	//SetPrimology(graphics::TRIANGLE_LIST);
}

// Surface::Surface(u32 aStartVertex, u32 aVertexCount, u32 aStartIndex, u32 anIndexCount, graphics::eTopology aPrimology)
// {
// 	SetVertexCount(aVertexCount);
// 	SetVertexStart(aStartVertex);
// 	SetIndexCount(anIndexCount);
// 	SetIndexStart(aStartIndex);
// 	//SetPrimology(aPrimology);
// }

void Surface::Activate(const graphics::RenderContext& /*render_context*/)
{
	if ( !myEffect )
		return;

	//render_context.GetContext().IASetTopology(graphics::eTopology::TRIANGLE_LIST);
	m_Material.Use(myEffect);
}

void Surface::Deactivate()
{
	if ( !myEffect )
		return;
	myEffect->Clear();
}

void Surface::AddTexture(const std::string& file_path, Effect::TextureSlot slot)
{
	std::string sub = file_path;
	if (file_path.find(".dds") == file_path.npos)
	{
		DL_WARNING("Incorrect filetype! %s", file_path.c_str());

		size_t pos = file_path.find(".");
		sub = file_path.substr(0, pos);
		sub += ".dds";
	}

	m_Material.AddResource(Engine::GetInstance()->GetTexture(sub.c_str()), sub, slot);
	
}

void Surface::AddTexture(IShaderResourceView* texture, const std::string& filepath, Effect::TextureSlot slot)
{
	m_Material.AddResource(texture, filepath, slot);
}

void Surface::AddTexture(Texture* texture, const std::string& filepath, Effect::TextureSlot slot)
{
	m_Material.AddResource(texture, filepath, slot);
}

void Surface::SetEffect(Effect* anEffect)
{
	myEffect = anEffect;
}

void Surface::SetVertexStart(u32 aStartVertex)
{
	myVertexStart = aStartVertex;
}

void Surface::SetVertexCount(u32 aVertexCount)
{
	myVertexCount = aVertexCount;
}

void Surface::SetIndexStart(u32 aStartIndex)
{
	myIndexStart = aStartIndex;
}

void Surface::SetIndexCount(u32 aIndexCount)
{
	myIndexCount = aIndexCount;
}

void Surface::serialize(const char* out)
{
	m_Material.Serialize(out);
}

// void Surface::SetPrimology(graphics::eTopology topology)
// {
// 	m_Topology = topology;
// }
// 
