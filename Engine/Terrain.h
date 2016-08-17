#pragma once
#include "BaseModel.h"
#include <standard_datatype.hpp>
namespace Snowblind
{
	class CSurface;

	struct SHeightMap
	{
		static SHeightMap* Create(const char* aFilePath);
		SHeightMap(u32 aWidth, u32 aDepth, u8* const someData);
		SHeightMap(std::fstream& aStream);
		void operator=(SHeightMap&) = delete;
		u32 myWidth;
		u32 myDepth;
		u8* myData;
	};

	class CTerrain : public CBaseModel
	{
	public:
		CTerrain(u32 width, u32 height);
		CTerrain(const std::string& aFilePath);
		CTerrain();
		~CTerrain();

		void Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection) override;

		void Save(const std::string& aFilename);
		void Load(const std::string& aFilePath);


	private:
		void CreateVertices(u32 width, u32 height);
		void SetMatrices(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection) override;
		void InitConstantBuffer();
		CU::Matrix44f myOrientation;
		u32 myWidth;
		u32 myHeight;
		SHeightMap* myHeightmap;

		CSurface* mySurface = nullptr;


		struct TerrainConstantStruct : public SVertexBaseStruct
		{
			CU::Vector4f time;
		} *myConstantStruct;

	};
};