#pragma once
#include <Engine/Material.h>

class Texture;
class Surface
{
public:

	Surface(Effect* anEffect);
	Surface(Effect* anEffect, uint32 aStartVertex, uint32 aVertexCount, uint32 aStartIndex, uint32 aIndexCount);
	//Surface(u32 aStartVertex, u32 aVertexCount, u32 aStartIndex, u32 anIndexCount, graphics::eTopology aPrimology);

	void AddTexture(IShaderResourceView* texture, const std::string& filepath, TextureSlot slot);
	void AddTexture(Texture* texture, const std::string& filepath, TextureSlot slot);
	void AddTexture(const std::string& file_path, TextureSlot slot);

	void Activate(const graphics::RenderContext& render_context);
	void Deactivate();

	void SetEffect(Effect* anEffect);

	void SetVertexStart(uint32 aStartVertex);
	void SetVertexCount(uint32 aVertexCount);

	void SetIndexStart(uint32 aStartIndex);
	void SetIndexCount(uint32 aIndexCount);

	//void SetPrimology(graphics::eTopology topology);

	int GetIndexCount() const;
	int GetVertexCount() const;
	int GetStartVertex() const;
	int GetStartIndex() const;

	void serialize(const char* out);
	void SetMaterial(Material copy);
	const Material& GetMaterial() const;
private:

	//graphics::eTopology m_Topology;
	Material m_Material;
	Effect* myEffect;

	uint32 myIndexStart;
	uint32 myIndexCount;
	uint32 myVertexStart;
	uint32 myVertexCount;

};

__forceinline int Surface::GetIndexCount() const
{
	return myIndexCount;
}

__forceinline int Surface::GetVertexCount() const
{
	return myVertexCount;
}

__forceinline int Surface::GetStartVertex() const
{
	return myVertexStart;
}

__forceinline int Surface::GetStartIndex() const
{
	return myIndexStart;
}

