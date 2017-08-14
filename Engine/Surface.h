#pragma once
#include <DataStructures/GrowingArray.h>
#include <string>
#include <Engine/Effect.h>
#include <Engine/engine_shared.h>
struct ID3D11DeviceContext;
enum D3D_PRIMITIVE_TOPOLOGY;

class Texture;

class Material
{
public:
	Material() = default;

	void AddResource(IShaderResourceView* pResource, Effect::TextureSlot slot);
	void AddResource(Texture* pResource, Effect::TextureSlot slot);

	void Use(Effect* pEffect, const RenderContext& render_context);
private:
	struct ResourceBinding
	{
		IShaderResourceView* m_Resource;
		Effect::TextureSlot m_Slot;
	};

	CU::GrowingArray<ResourceBinding> m_Resources;
};



class Surface
{
public:
	Surface(Effect* anEffect);
	Surface(Effect* anEffect, u32 aStartVertex, u32 aVertexCount, u32 aStartIndex, u32 aIndexCount);
	Surface(u32 aStartVertex, u32 aVertexCount, u32 aStartIndex, u32 anIndexCount, D3D_PRIMITIVE_TOPOLOGY aPrimology);

	void AddTexture(IShaderResourceView* texture, Effect::TextureSlot slot);
	void AddTexture(Texture* texture, Effect::TextureSlot slot);
	void AddTexture(const std::string& file_path, Effect::TextureSlot slot);

	void Activate(const RenderContext& render_context);
	void Deactivate();

	void SetEffect(Effect* anEffect);

	void SetVertexStart(u32 aStartVertex);
	void SetVertexCount(u32 aVertexCount);

	void SetIndexStart(u32 aStartIndex);
	void SetIndexCount(u32 aIndexCount);

	void SetPrimology(D3D_PRIMITIVE_TOPOLOGY aPrimology);

	int GetIndexCount() const;
	int GetVertexCount() const;
	int GetStartVertex() const;
	int GetStartIndex() const;


private:

	D3D_PRIMITIVE_TOPOLOGY myPrimologyType;
	
	Material m_Material;
	Effect* myEffect;

	u32 myIndexStart;
	u32 myIndexCount;
	u32 myVertexStart;
	u32 myVertexCount;

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

