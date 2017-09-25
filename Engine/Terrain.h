#pragma once
#include "BaseModel.h"

class Surface;

struct SHeightMap
{
	u32 myWidth;
	u32 myDepth;
	u8* myData;
};

SHeightMap Create(const char* filepath);

class Terrain : public BaseModel
{
public:
	Terrain() = default;

	bool Initiate(const std::string& aFile, const CU::Vector3f position, const CU::Vector2f& aSize);
	void CleanUp();
	void Render(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context) override;
	void ShadowRender(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context) override;

	void Render(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context, bool override_shader);


	void Save(const std::string& aFilename);
	void Load(const std::string& filepath);

	void AddNormalMap(const std::string& filepath);
	std::vector<float> GetVerticeArrayCopy();
	std::vector<s32> GetIndexArrayCopy();
	void SetPosition(CU::Vector2f position);


	bool HasLoaded() const { return m_HasLoaded; }
private:
	bool m_HasLoaded = false;
	void UpdateConstantBuffer(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context) override;

	void CreateVertices(u32 width, u32 height, const CU::Vector3f& position);
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

	Surface* mySurface = nullptr;

	VertexBaseStruct myConstantStruct;

	Effect* m_ClipEffect = nullptr;

};
