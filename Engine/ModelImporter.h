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

	CModel* LoadModel(std::string aFilePath, CModel* model, std::string aEffectPath);

private:
	Engine* m_Engine = nullptr;
	Ticket_Mutex m_LoaderMutex;


	std::string m_CurrentFile;
	CommonUtilities::TimeManager* m_TimeManager;
	
	//Should be removed from loader
	CU::Vector3f m_WHD;
	CU::Vector3f m_MinPoint;
	CU::Vector3f m_MaxPoint;
	//Assimp::Importer m_Importer; //If we move this as well then we might be able to load several models at the same time?

	CModel* CreateModel(FBXModelData* someData, CModel* model, Effect* anEffect);
	CModel* CreateChild(FBXModelData* data, Effect* effect);


	CModel* LoadModel(std::string aFilePath, CModel* model, Effect* anEffect);
	void FillData(FBXModelData* someData, CModel* out, Effect* anEffect);
	void ProcessNode(aiNode* aNode, const aiScene* aScene, FBXModelData* someData, std::string file);
	void ProcessMesh(aiMesh* aMesh, const aiScene* aScene, FBXModelData* fbx, std::string file);
};
