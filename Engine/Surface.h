#pragma once
#include <DataStructures/GrowingArray.h>
#include <string>
struct ID3DX11EffectShaderResourceVariable;
enum D3D_PRIMITIVE_TOPOLOGY;
namespace Snowblind
{
	class CTexture;
	class CEffect;

	class CSurface
	{
	public:
		CSurface(CEffect* anEffect);
		CSurface(CEffect* anEffect, unsigned int aStartVertex, unsigned int aVertexCount,
			unsigned int aStartIndex, unsigned int aIndexCount);
		CSurface(unsigned int aStartVertex, unsigned int aVertexCount, unsigned int aStartIndex, unsigned int anIndexCount, D3D_PRIMITIVE_TOPOLOGY aPrimology);

		~CSurface();

		void Activate();
		void SetTexture(const std::string& aResourceName, const std::string& aFilePath);

		void SetEffect(CEffect* anEffect);

		void SetVertexStart(unsigned int aStartVertex);
		void SetVertexCount(unsigned int aVertexCount);

		void SetIndexStart(unsigned int aStartIndex);
		void SetIndexCount(unsigned int aIndexCount);

		void SetPrimology(D3D_PRIMITIVE_TOPOLOGY aPrimology);

		int GetIndexCount() const;
		int GetVertexCount() const;
		int GetStartVertex() const;
		int GetStartIndex() const;
		CTexture* GetTexture();
	private:

		D3D_PRIMITIVE_TOPOLOGY myPrimologyType;

		struct STexture
		{
			CTexture* texture;
			std::string resourceName;
		};

		CU::GrowingArray<ID3DX11EffectShaderResourceVariable*> myShaderVariables;
		CU::GrowingArray<STexture*> myTextures;
		CU::GrowingArray<std::string> myResourceNames;
		CU::GrowingArray<std::string> myFileNames;

		CEffect* myEffect;

		unsigned int myIndexStart;
		unsigned int myIndexCount;
		unsigned int myVertexStart;
		unsigned int myVertexCount;

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
