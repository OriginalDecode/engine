#pragma once
#include <standard_datatype.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "snowblind_shared.h"
#include "Surface.h"
namespace Snowblind
{
	class CModel;
	class CEffect;
	class CEngine;
};

namespace CommonUtilities
{
	class TimeManager;
}

struct TextureInfo
{
	Snowblind::TextureType myType;
	std::string myFilename;
};

struct Vertex
{
	CU::Vector4f position;
	CU::Vector4f normal;
	CU::Vector2f uv;
	CU::Vector4f binormal;
	CU::Vector4f tangent;
};

bool operator==(const Vertex& aVertice, const Vertex& aSecond);
bool operator!=(const Vertex& aVertice, const Vertex& aSecond);

struct ModelData
{
	enum LayoutType
	{
		VERTEX_POS,
		VERTEX_NORMAL,
		VERTEX_UV,
		VERTEX_BINORMAL,
		VERTEX_TANGENT,
		VERTEX_SKINWEIGHTS,
		VERTEX_BONEID,
	};

	struct Layout
	{
		int mySize;
		int myOffset;
		LayoutType myType;
	};

	float* myVertexBuffer;
	u32* myIndicies = nullptr;
	u32 myVertexCount = 0;
	u32 myVertexStride = 0; //What is this?
	u32 myIndexCount = 0;
	CU::GrowingArray<TextureInfo> myTextures;
	CU::GrowingArray<Layout> myLayout;

	CU::Vector3f m_WHD;
};

struct TextureData
{
	CU::GrowingArray<TextureInfo> myTextures;
};

struct FBXModelData
{
	CU::Matrix44f myOrientation;
	ModelData* myData = nullptr;
	TextureData* myTextureData = nullptr;
	CU::GrowingArray<FBXModelData*> myChildren;
};


class CModelImporter
{
public:
	CModelImporter();
	~CModelImporter();

	Snowblind::CModel* LoadModel(const std::string& aFilePath, const std::string& aEffectPath);

private:
	Ticket_Mutex m_LoaderMutex;
	Assimp::Importer importer;
	Snowblind::CEngine* myEngine;
	std::string myCurrentLoadingFile;
	CommonUtilities::TimeManager* myTimeManager;

	Snowblind::CModel* CreateModel(FBXModelData* someData, Snowblind::CEffect* anEffect);
	Snowblind::CModel* LoadModel(const std::string& aFilePath, Snowblind::CEffect* anEffect);
	void FillData(FBXModelData* someData, Snowblind::CModel* out, Snowblind::CEffect* anEffect);
	void ProcessNode(aiNode* aNode, const aiScene* aScene, FBXModelData* someData);
	void ProcessMesh(aiMesh* aMesh, const aiScene* aScene, FBXModelData* fbx);
};
