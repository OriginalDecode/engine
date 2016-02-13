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
		~CSurface();

		void Activate();
		void SetTexture(const std::string& aResourceName, const std::string& aFilePath);

		
		void SetVertexStart(unsigned int aStartVertex);
		void SetVertexCount(unsigned int aVertexCount);
		
		void SetIndexStart(unsigned int aStartIndex);
		void SetIndexCount(unsigned int aIndexCount);

		void SetPrimology(D3D_PRIMITIVE_TOPOLOGY aPrimology);


	private:

		D3D_PRIMITIVE_TOPOLOGY myPrimologyType;

		CU::GrowingArray<ID3DX11EffectShaderResourceVariable*> myShaderVariables;
		CU::GrowingArray<CTexture*> myTextures;
		CU::GrowingArray<std::string> myResourceNames;
		CU::GrowingArray<std::string> myFileNames;

		CEffect	*myEffect;


		unsigned int myIndexStart;
		unsigned int myIndexCount;
		unsigned int myVertexStart;
		unsigned int myVertexCount;

	};
};