#pragma once
#include <assimp/Importer.hpp>

#include <Engine/Surface.h>
#include "snowblind_shared.h"
#ifdef _DEBUG
#include <TimeManager.h>
#endif
class Model;
class Effect;
class Engine;

struct TextureInfo
{
	TextureType myType;
	std::string myFilename;
};

class aiNode;
class aiMesh;
class aiScene;


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

	void LoadModel(std::string filepath, Model* model, std::string aEffectPath);

private:
	Engine* m_Engine = nullptr;
	Ticket_Mutex m_LoaderMutex;

#ifdef _DEBUG
	CU::TimeManager m_TimeManager;
#endif

	Model* CreateModel(FBXModelData* data, Model* model, std::string filepath, Effect* effect);
	Model* CreateChild(FBXModelData* data, std::string filepath, Effect* effect);


	Model* LoadModel(std::string filepath, Model* model, Effect* effect);

	void FillData(FBXModelData* data, Model* model, std::string filepath);

	void SetupInputLayout(ModelData* data, Model* model);

	void ProcessNode(aiNode* node, const aiScene* scene, FBXModelData* data, std::string file);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, FBXModelData* data, std::string file);

	void ExtractMaterials(aiMesh* mesh,const aiScene* scene, FBXModelData* data, std::string file);

};
