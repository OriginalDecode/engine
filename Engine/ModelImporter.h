#pragma once
#include <standard_datatype.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "snowblind_shared.h"
#include "Surface.h"

class CModel;
class Effect;
class Engine;

namespace CommonUtilities
{
	class TimeManager;
}

struct TextureInfo
{
	TextureType myType;
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

	CU::Vector3f m_MinPoint;
	CU::Vector3f m_MaxPoint;


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

	CModel* LoadModel(const std::string& aFilePath, const std::string& aEffectPath);

private:
	Ticket_Mutex m_LoaderMutex;
	Assimp::Importer importer;
	Engine* myEngine;
	std::string myCurrentLoadingFile;
	CommonUtilities::TimeManager* myTimeManager;
	CU::Vector3f m_WHD;
	CU::Vector3f m_MinPoint;
	CU::Vector3f m_MaxPoint;
	CModel* CreateModel(FBXModelData* someData, Effect* anEffect);
	CModel* LoadModel(const std::string& aFilePath, Effect* anEffect);
	void FillData(FBXModelData* someData, CModel* out, Effect* anEffect);
	void ProcessNode(aiNode* aNode, const aiScene* aScene, FBXModelData* someData);
	void ProcessMesh(aiMesh* aMesh, const aiScene* aScene, FBXModelData* fbx);
};
