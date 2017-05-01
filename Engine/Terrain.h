#pragma once
#include "BaseModel.h"
#include <standard_datatype.hpp>

class CSurface;

struct SHeightMap
{
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
	void Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const RenderContext& render_context) override;
	void ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const RenderContext& render_context) override;


	void Save(const std::string& aFilename);
	void Load(const std::string& aFilePath);

	void AddNormalMap(const std::string& filepath);
	std::vector<float> GetVerticeArrayCopy();
	std::vector<s32> GetIndexArrayCopy();
	void SetPosition(CU::Vector2f position);


	bool HasLoaded() const { return m_HasLoaded; }
private:
	bool m_HasLoaded = false;

	void UpdateConstantBuffer(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection) override;

	void CreateVertices(u32 width, u32 height, const CU::Vector3f& position);
	void InitConstantBuffer();

	void CalculateNormals(CU::GrowingArray<SVertexPosNormUVBiTang>& VertArray);
	float GetHeight(unsigned int aX, unsigned int aY) const;
	float GetHeight(unsigned int aIndex) const;

	SHeightMap myHeightmap;

	/* Look into this */
	std::vector<s32> myIndexes;
	std::vector<float> myVertices;


	CU::Matrix44f myOrientation;
	u32 myWidth;
	u32 myDepth;

	CSurface* mySurface = nullptr;

	struct TerrainConstantStruct : public VertexBaseStruct
	{
		CU::Vector4f time;
	} myConstantStruct;


	ID3D11Buffer* m_PSConstantBuffer;
	struct TerrainCameraPos
	{
		CU::Vector4f camPos;
	} m_PSConstantStruct;

};
