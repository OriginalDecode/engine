#pragma once
#include <DataStructures/GrowingArray.h>
#include <string>
#include <Engine/Effect.h>
#include <Engine/snowblind_shared.h>
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
	void Clear();

private:
	struct ResourceBinding
	{
		IShaderResourceView* m_Resource;
		Effect::TextureSlot m_Slot;
	};

	CU::GrowingArray<ResourceBinding> m_Resources;
};



class CSurface
{
public:
	CSurface(Effect* anEffect);
	CSurface(Effect* anEffect, u32 aStartVertex, u32 aVertexCount,
		u32 aStartIndex, u32 aIndexCount);
	CSurface(u32 aStartVertex, u32 aVertexCount, u32 aStartIndex, u32 anIndexCount, D3D_PRIMITIVE_TOPOLOGY aPrimology);

	~CSurface();

	void ClearTextures();
	void AddTexture(IShaderResourceView* texture);
	void AddTexture(const std::string& file_path);

	void Activate();
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

	//void ValidateTextures();
	//void RemoveTextureByIndex(s32 index);

private:
	//void AddMissingTexture(TextureType type, const std::string& file_path);

	D3D_PRIMITIVE_TOPOLOGY myPrimologyType;
	
	Material m_Material;

	Effect* myEffect;
	ID3D11DeviceContext* myContext;
	u32 myIndexStart;
	u32 myIndexCount;
	u32 myVertexStart;
	u32 myVertexCount;

};

__forceinline int CSurface::GetIndexCount() const
{
	return myIndexCount;
}

__forceinline int CSurface::GetVertexCount() const
{
	return myVertexCount;
}

__forceinline int CSurface::GetStartVertex() const
{
	return myVertexStart;
}

__forceinline int CSurface::GetStartIndex() const
{
	return myIndexStart;
}

