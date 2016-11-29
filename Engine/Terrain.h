#pragma once
#include "BaseModel.h"
#include <standard_datatype.hpp>
namespace Snowblind
{
	class CSurface;

	struct SHeightMap
	{
		/*SHeightMap(u32 aWidth, u32 aDepth, u8* const someData);
		SHeightMap(std::fstream& aStream);*/
		u32 myWidth;
		u32 myDepth;
		u8* myData;
	};
	SHeightMap Create(const char* aFilePath);

	class CTerrain : public CBaseModel
	{
	public:
		CTerrain() = default;
		
		bool Initiate(const std::string& aFile, const CU::Vector3f position, const CU::Vector2f& aSize);
		bool CleanUp() override;
		void Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, bool render_shadows = false) override;

		void Save(const std::string& aFilename);
		void Load(const std::string& aFilePath);
		//CU::GrowingArray<float> GetVerticeArrayCopy() { return myVertices; };
		//CU::GrowingArray<s32> GetIndexArrayCopy() {	return myIndexes; };
		void AddNormalMap(const std::string& filepath);
		std::vector<float> GetVerticeArrayCopy();
		std::vector<s32> GetIndexArrayCopy();
		void SetPosition(CU::Vector2f position);


		bool HasLoaded() const { return m_HasLoaded; }
	private:
		bool m_HasLoaded = false;

		void SetMatrices(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection) override;

		void CreateVertices(u32 width, u32 height, const CU::Vector3f& position);
		void InitConstantBuffer();

		void CalculateNormals(CU::GrowingArray<SVertexPosNormUVBiTang>& VertArray);
		float GetHeight(unsigned int aX, unsigned int aY) const;
		float GetHeight(unsigned int aIndex) const;
		
		SHeightMap myHeightmap;
		std::vector<s32> myIndexes;
		std::vector<float> myVertices;

		CU::Matrix44f myOrientation;
		u32 myWidth;
		u32 myDepth;

		CSurface* mySurface = nullptr;

		struct TerrainConstantStruct : public SVertexBaseStruct
		{
			CU::Vector4f time;
		} *myConstantStruct;

		
		ID3D11Buffer* m_PSConstantBuffer;
		struct TerrainCameraPos
		{
			CU::Vector4f camPos;
		} *m_PSConstantStruct;

	};
};