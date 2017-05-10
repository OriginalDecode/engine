#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Engine/Surface.h>
#include "snowblind_shared.h"
#ifdef _DEBUG
#include <TimeManager.h>
#endif

#include <Engine/Model.h>
#include <Engine/LightModel.h>
#include <Engine/AtmosphereModel.h>


#include <Engine/VertexWrapper.h>
#include <Engine/IndexWrapper.h>

#include <Engine/Effect.h>


class Engine;
class aiNode;
class aiMesh;
class aiScene;

class CModelImporter
{
public:
	CModelImporter();

	//void LoadModel(std::string filepath, Model* model, std::string aEffectPath);
	
	template<typename T>
	void LoadModel(T* pModel, std::string model_filepath, std::string effect_filepath);

private:
	Engine* m_Engine = nullptr;
	Ticket_Mutex m_LoaderMutex;
#pragma region Structs
	struct TextureInfo //Anyway to remove this?
	{
		Effect::TextureSlot m_Slot;
		std::string m_File;
	};

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
			s32 mySize;
			s32 myOffset;
			LayoutType myType;
		};

		float* myVertexBuffer;
		u32* myIndicies = nullptr;
		u32 myVertexCount = 0;
		u32 myVertexStride = 0; //byte distance between each vertex
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
		void DeleteChildren();

	};
#pragma endregion

#ifdef _DEBUG
	CU::TimeManager m_TimeManager;
#endif

	template<typename T>
	void CreateModel(FBXModelData* data, T* model, std::string filepath, Effect* effect);
	template<typename T>
	T* CreateChild(FBXModelData* data, std::string filepath, Effect* effect);

	template<typename T>
	void LoadModel(std::string filepath, T* pModel, Effect* effect);

	template<typename T>
	void FillData(FBXModelData* data, T* model, std::string filepath, Effect* effect);

	template<typename T>
	void SetupInputLayout(ModelData* data, T* model);

	void ProcessNode(aiNode* node, const aiScene* scene, FBXModelData* data, std::string file);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, FBXModelData* data, std::string file);

	void ExtractMaterials(aiMesh* mesh,const aiScene* scene, FBXModelData* data, std::string file);
};

template<typename T>
void CModelImporter::LoadModel(T* pModel, std::string model_filepath, std::string effect_filepath)
{
	BeginTicketMutex(&m_LoaderMutex);

	LoadModel(model_filepath, pModel, m_Engine->GetEffect(effect_filepath));

	EndTicketMutex(&m_LoaderMutex);
}

template<typename T>
void CModelImporter::LoadModel(std::string filepath, T* pModel, Effect* effect)
{

#ifdef _DEBUG
	m_TimeManager.Update();
	float loadTime = m_TimeManager.GetTimer(0).GetTotalTime().GetMilliseconds();
#endif
	unsigned int processFlags =
		aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
									 //aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
									 //aiProcess_ValidateDataStructure  | // perform a full validation of the loader's output
									 //aiProcess_Triangulate | // Ensure all verticies are triangulated (each 3 vertices are triangle)
									 //aiProcess_ConvertToLeftHanded | // convert everything to D3D left handed space (by default right-handed, for OpenGL)
									 //aiProcess_SortByPType | // ?
									 //aiProcess_ImproveCacheLocality | // improve the cache locality of the output vertices
									 //aiProcess_RemoveRedundantMaterials | // remove redundant materials
									 //aiProcess_FindDegenerates | // remove degenerated polygons from the import
									 //aiProcess_FindInvalidData | // detect invalid model data, such as invalid normal vectors
									 //aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
									 ////aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
									 //aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
									 //aiProcess_LimitBoneWeights | // limit bone weights to 4 per vertex
		aiProcess_OptimizeMeshes | // join small meshes, if possible;
								   //aiProcess_OptimizeGraph |
								   //aiProcess_SplitByBoneCount | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
		0;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, processFlags);

	DL_MESSAGE_EXP(!scene, "%s", importer.GetErrorString());
	DL_ASSERT_EXP(scene, "ImportModel Failed. Could not read the requested file.");

	aiNode* rootNode = scene->mRootNode;
	FBXModelData* data = new FBXModelData;

	ProcessNode(rootNode, scene, data, filepath);

	CreateModel(data, pModel, filepath, effect);


	if (data->myTextureData)
	{
		delete data->myTextureData;
	}
	if (data->myData)
	{
		delete[] data->myData->myIndicies;
		delete[] data->myData->myVertexBuffer;
	}
	if (data)
	{
		delete data->myData;
	}

	for (FBXModelData* child : data->myChildren)
	{
	}

	delete data;

#ifdef _DEBUG
	m_TimeManager.Update();
	loadTime = m_TimeManager.GetTimer(0).GetTotalTime().GetMilliseconds() - loadTime;
	MODEL_LOG("%s took %fms to load. %s", filepath.c_str(), loadTime, (loadTime > 7000.f) ? "Check if it's saved as binary." : 0);
#endif
}

template<typename T>
void CModelImporter::CreateModel(FBXModelData* someData, T* model, std::string filepath, Effect* effect)
{
	model->SetEffect(effect);

	if (someData->myData)
	{
		FillData(someData, model, filepath, effect);
		model->myOrientation = someData->myOrientation;
	}

	for (FBXModelData* child : someData->myChildren)
	{
		model->AddChild(CreateChild<T>(child, filepath, effect));
	}

}

template<typename T>
T* CModelImporter::CreateChild(FBXModelData* data, std::string filepath, Effect* effect)
{
	T* model = new T;

	size_t pos = filepath.rfind('/');

	model->m_Filename = filepath.substr(pos);
	model->SetEffect(effect);

	if (data->myData)
	{
		FillData(data, model, filepath, effect);
		model->myOrientation = data->myOrientation;
	}

	for (FBXModelData* child : data->myChildren)
	{
		model->AddChild(CreateChild<T>(child, filepath, effect));
	}

	return model;
}

template<typename T>
void CModelImporter::FillData(FBXModelData* someData, T* out, std::string filepath, Effect* effect)
{
	ModelData* data = someData->myData;

	out->m_IndexData.myFormat = DXGI_FORMAT_R32_UINT;
	u32* indexData = new u32[data->myIndexCount];
	memcpy(indexData, data->myIndicies, data->myIndexCount * sizeof(u32));
	out->m_IndexData.myIndexData = (s8*)indexData;
	out->m_IndexData.mySize = data->myIndexCount * sizeof(u32);
	out->m_IndexData.myIndexCount = data->myIndexCount;
	for (u32 i = 0; i < data->myIndexCount; i++)
	{
		out->m_Indices.Add(data->myIndicies[i]);
	}

	s32 sizeOfBuffer = data->myVertexCount * data->myVertexStride * sizeof(float); //is this wrong?
	u32* vertexRawData = new u32[sizeOfBuffer];
	memcpy(vertexRawData, data->myVertexBuffer, sizeOfBuffer); // This crashes?
	out->m_VertexData.myVertexData = (s8*)vertexRawData;
	out->m_VertexData.myNrOfVertexes = data->myVertexCount;
	out->m_VertexData.mySize = sizeOfBuffer;
	out->m_VertexData.myStride = data->myVertexStride * sizeof(float);

	CSurface* newSurface = new CSurface(0, data->myVertexCount, 0
		, data->myIndexCount, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	out->m_IsRoot = false;
	SetupInputLayout(data, out);
	newSurface->SetEffect(effect);

	const CU::GrowingArray<TextureInfo>& info = someData->myTextureData->myTextures;

	for (s32 i = 0; i < info.Size(); i++)
	{
		newSurface->AddTexture(info[i].m_File, info[i].m_Slot);
	}


	out->mySurfaces.Add(newSurface);

}

template<typename T>
void CModelImporter::SetupInputLayout(ModelData* data, T* out)
{
	for (int i = 0; i < data->myLayout.Size(); ++i)
	{
		auto currentLayout = data->myLayout[i];
		D3D11_INPUT_ELEMENT_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_INPUT_ELEMENT_DESC));
		desc.SemanticIndex = 0;
		desc.AlignedByteOffset = currentLayout.myOffset;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InputSlot = 0;
		desc.InstanceDataStepRate = 0;

		if (currentLayout.myType == ModelData::VERTEX_POS)
		{
			desc.SemanticName = "POSITION";
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_NORMAL)
		{
			desc.SemanticName = "NORMAL";
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_UV)
		{
			desc.SemanticName = "TEXCOORD";
			desc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_BINORMAL)
		{
			desc.SemanticName = "BINORMAL";
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_TANGENT)
		{
			desc.SemanticName = "TANGENT";
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_SKINWEIGHTS)
		{
			break;
			desc.SemanticName = "WEIGHTS";
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_BONEID)
		{
			break;
			desc.SemanticName = "BONES";
			desc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
		}
		out->myVertexFormat.Add(desc);
	}
}

