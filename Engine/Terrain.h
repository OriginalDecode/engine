#pragma once
#include "BaseModel.h"
#include "ConstantBuffer.h"

class Surface;
constexpr float TERRAIN_HALFWIDTH = 1024.f;

namespace
{
	const char* GrayTile = "Data/Textures/GrayTile.dds";
	const char* Britannia = "Data/Textures/terrain/britannia.dds";
	const char* BritanniaNormal = "Data/Textures/terrain/brit_n.dds";
	const char* Flat = "Data/Textures/flat_height.dds";
	const char* Grass = "Data/Material/grass/grass1-albedo3.dds";
	const char* Test = "Data/Textures/terrain/test.dds";
	const char* Canyon = "Data/Textures/terrain/Heightmap.png";
};
#define ALBEDO GrayTile
#define HEIGHTMAP Britannia


struct SHeightMap
{
	uint32 myWidth;
	uint32 myDepth;
	uint8* myData;
};

SHeightMap Create(const char* filepath);

class Terrain : public BaseModel
{
public:

	Terrain() = default;
	Terrain(float halfwidth, CU::Vector2f tex[4], CU::Vector3f color = { 0,0,0 });
	Terrain(float halfwidth, CU::Vector3f color = { 0,0,0 });

	bool Initiate(const std::string& aFile, const CU::Vector3f position, const CU::Vector2f& aSize);
	void CleanUp();
	void Render(const graphics::RenderContext& rc) override;
	void Wireframe(const graphics::RenderContext& rc);
	void ShadowRender(const graphics::RenderContext& rc) override;

	void Render(const graphics::RenderContext& rc, bool override_shader);
	void Render(const graphics::RenderContext& rc, bool override_shader, bool skip_binding);


	void Save(const std::string& aFilename);
	void Load(const std::string& filepath);

	void AddNormalMap(const std::string& filepath);
	std::vector<float> GetVerticeArrayCopy();
	std::vector<int32> GetIndexArrayCopy();
	void SetPosition(CU::Vector2f position);
	bool HasLoaded() const { return m_HasLoaded; }

	const CU::Matrix44f& GetOrientation() const override { return m_Orientation; }
	CU::Matrix44f& GetOrientation() { return m_Orientation; }

	void SetOrientation(const CU::Matrix44f m) override {};

	void SetForward(const CU::Vector4f& d) override { m_Orientation.SetForward(d); }
	void SetPosition(const CU::Vector4f& v) override { m_Orientation.SetPosition(v); }

	Effect* GetEffect() { return m_Effect; }
	static bool draw_wireframe;
private:

	float m_HalfWidth = 0.f;
	void SetupTextures();

	void AddSurface(Surface* p) override { delete p; p = nullptr; }

	bool m_HasLoaded = false;
	void UpdateConstantBuffer(const graphics::RenderContext&) { };

	void CreateVertices(uint32 width, uint32 height, const CU::Vector3f& position);
	void CalculateNormals(CU::GrowingArray<SVertexPosNormUVBiTang>& VertArray);
	float GetHeight(unsigned int aX, unsigned int aY) const;
	float GetHeight(unsigned int aIndex) const;

	void CreatePlane(float halfwidth) override;


	SHeightMap myHeightmap;

	/* Look into this */
	std::vector<int32> myIndexes;
	std::vector<float> myVertices;

	CU::Matrix44f myOrientation;
	uint32 myWidth;
	uint32 myDepth;
	CU::Vector3f m_Color;
	CU::Vector2f m_UV[4];


	graphics::ConstantBuffer m_Buffer;
	graphics::ConstantBuffer m_PixelBuffer;
	//VertexBaseStruct myConstantStruct;

	Effect* m_ClipEffect = nullptr;

	

};
