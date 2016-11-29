#pragma once
#include <DataStructures/GrowingArray.h>
#include <string>
struct ID3D11DeviceContext;
enum D3D_PRIMITIVE_TOPOLOGY;
namespace Snowblind
{
	class Texture;
	class CEffect;

	enum TextureType
	{
		_ALBEDO = 1,
		_NORMAL = 2,
		_ROUGHNESS = 4,
		_METALNESS = 8,
		_EMISSIVE = 16,
		_AO = 32,
	};


	class CSurface
	{
	public:
		CSurface(CEffect* anEffect);
		CSurface(CEffect* anEffect, u32 aStartVertex, u32 aVertexCount,
			u32 aStartIndex, u32 aIndexCount);
		CSurface(u32 aStartVertex, u32 aVertexCount, u32 aStartIndex, u32 anIndexCount, D3D_PRIMITIVE_TOPOLOGY aPrimology);

		~CSurface();

		void Activate();
		void Deactivate();
		void AddTexture(const std::string& file_path, TextureType type);

		void SetEffect(CEffect* anEffect); 

		void SetVertexStart(u32 aStartVertex);
		void SetVertexCount(u32 aVertexCount);

		void SetIndexStart(u32 aStartIndex);
		void SetIndexCount(u32 aIndexCount);

		void SetPrimology(D3D_PRIMITIVE_TOPOLOGY aPrimology);

		int GetIndexCount() const;
		int GetVertexCount() const;
		int GetStartVertex() const;
		int GetStartIndex() const;

		void ValidateTextures();
		void RemoveTextureByIndex(s32 index);
	private:
		void AddMissingTexture(TextureType type, const std::string& file_path);

		D3D_PRIMITIVE_TOPOLOGY myPrimologyType;
		s32 m_ContainingTextures = 0;

		struct STexture
		{
			IShaderResourceView* texture;
			TextureType m_Type;
		};

		CU::GrowingArray<STexture> myTextures;

		CU::GrowingArray<IShaderResourceView*> myShaderViews;
		CU::GrowingArray<IShaderResourceView*> m_Null;

		CU::GrowingArray<std::string> myFileNames;

		CEffect* myEffect;
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

};
