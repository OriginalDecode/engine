#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Engine/Surface.h>
#include "engine_shared.h"
#ifdef _DEBUG
#include <TimeManager.h>
#endif

#include <Engine/Model.h>
#include <Engine/LightModel.h>
#include <Engine/AtmosphereModel.h>


#include <Engine/VertexWrapper.h>
#include <Engine/IndexWrapper.h>

#include <Engine/Effect.h>


#include <Engine/IGraphicsDevice.h>
#include <Engine/IGraphicsContext.h>
#include <Engine/IGraphicsAPI.h>
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
	void FillInstanceData(T* out, ModelData* data, Effect* effect);

	template<typename T>
	void FillIndexData(T* out, ModelData* data);
	template<typename T>
	void FillVertexData(T* out, ModelData* data, Effect* effect);


	void SetupInputLayout(ModelData* data, CU::GrowingArray<graphics::InputElementDesc>& element_desc);

	void ProcessNode(aiNode* node, const aiScene* scene, FBXModelData* data, std::string file, CU::Vector3f& min_point, CU::Vector3f& max_point);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, FBXModelData* data, std::string file, CU::Vector3f& min_point, CU::Vector3f& max_point);

	void ExtractMaterials(aiMesh* mesh,const aiScene* scene, FBXModelData* data, std::string file);
};

template<typename T>
void CModelImporter::LoadModel(T* pModel, std::string model_filepath, std::string effect_filepath)
{
	BeginTicketMutex(&m_LoaderMutex);

	LoadModel(model_filepath, pModel, m_Engine->GetEffect(effect_filepath.c_str()));

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

	CU::Vector3f max_point, min_point;
	ProcessNode(rootNode, scene, data, filepath, min_point, max_point);

	CreateModel(data, pModel, filepath, effect);

	pModel->SetMaxPoint(max_point);
	pModel->SetMinPoint(min_point);

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
		model->m_Orientation = someData->myOrientation;
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

	//model->m_Filename = filepath.substr(pos);
	model->SetEffect(effect);


	if (data->myData)
	{
		FillData(data, model, filepath, effect);
		model->m_Orientation = data->myOrientation;
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

	FillVertexData(out, data, effect);
	FillIndexData(out, data);
	FillInstanceData(out, data, effect);

	Surface* newSurface = new Surface(0, data->myVertexCount, 0, data->myIndexCount, graphics::TRIANGLE_LIST);
	out->m_IsRoot = false;

	newSurface->SetEffect(effect);
	const CU::GrowingArray<TextureInfo>& info = someData->myTextureData->myTextures;

	for (s32 i = 0; i < info.Size(); i++)
	{
		newSurface->AddTexture(info[i].m_File, info[i].m_Slot);
	}

	out->m_Surfaces.Add(newSurface);


	out->SetMinPoint(data->m_MinPoint);
	out->SetMaxPoint(data->m_MaxPoint);

}

inline void CModelImporter::SetupInputLayout(ModelData* data, CU::GrowingArray<graphics::InputElementDesc>& element_desc)
{
 	for (int i = 0; i < data->myLayout.Size(); ++i)
 	{
 		auto currentLayout = data->myLayout[i];
		graphics::InputElementDesc desc;
 		desc.m_SemanicIndex = 0;
 		desc.m_ByteOffset = currentLayout.myOffset;
 		desc.m_ElementSpecification = graphics::INPUT_PER_VERTEX_DATA;
 		desc.m_InputSlot = 0;
 		desc.m_InstanceDataStepRate = 0;
 
 		if (currentLayout.myType == ModelData::VERTEX_POS)
 		{
			desc.m_Semantic = "POSITION";
			desc.m_Format = graphics::_16BYTE_RGBA;
 		}
 		else if (currentLayout.myType == ModelData::VERTEX_NORMAL)
 		{
 			desc.m_Semantic = "NORMAL";
 			desc.m_Format = graphics::_16BYTE_RGBA;
 		}
 		else if (currentLayout.myType == ModelData::VERTEX_UV)
 		{
			desc.m_Semantic = "TEXCOORD";
			desc.m_Format = graphics::_8BYTE_RG;
 		}
 		else if (currentLayout.myType == ModelData::VERTEX_BINORMAL)
 		{
			desc.m_Semantic = "BINORMAL";
			desc.m_Format = graphics::_16BYTE_RGBA;
 		}
 		else if (currentLayout.myType == ModelData::VERTEX_TANGENT)
 		{
			desc.m_Semantic = "TANGENT";
			desc.m_Format = graphics::_16BYTE_RGBA;
 		}
 		else if (currentLayout.myType == ModelData::VERTEX_SKINWEIGHTS)
 		{
 			break;
			desc.m_Semantic = "WEIGHTS";
 			desc.m_Format = graphics::_16BYTE_RGBA;
 		}
 		else if (currentLayout.myType == ModelData::VERTEX_BONEID)
 		{
 			break;
			desc.m_Semantic = "BONES";
 			desc.m_Format = graphics::_16BYTE_RGBA;
 		}
		element_desc.Add(desc);
 	}
}


template<typename T>
void CModelImporter::FillIndexData(T* out, ModelData* data)
{
	BaseModel* model = static_cast<BaseModel*>(out);
	auto& idx = model->m_IndexWrapper;
	s8* indexData = new s8[data->myIndexCount];
	memcpy(indexData, data->myIndicies, data->myIndexCount * sizeof(u32));
	const graphics::eTextureFormat idx_IndexBufferFormat = graphics::R32_UINT;
	const s32 idx_IndexCount = data->myIndexCount;
	const s32 idx_Size = idx_IndexCount * sizeof(u32);

	graphics::BufferDesc idx_desc;
	idx_desc.m_Size = idx_Size;
	idx_desc.m_Data = (void*)indexData;
	idx_desc.m_BindFlag = graphics::BIND_INDEX_BUFFER;
	idx_desc.m_UsageFlag = graphics::IMMUTABLE_USAGE;
	idx_desc.m_StructuredByteStride = 0;
	idx_desc.m_CPUAccessFlag = graphics::NO_ACCESS_FLAG;
	idx_desc.m_MiscFlags = 0;
	idx_desc.m_ByteWidth = idx_desc.m_Size;

	IBuffer* buffer = Engine::GetAPI()->GetDevice().CreateBuffer(idx_desc);

	//IndexWrapper(s8* data, s32 index_count, s32 start, s32 size, graphics::eTextureFormat format, s32 byte_offset, IBuffer* buffer)
	idx = IndexWrapper(indexData,
					   idx_IndexCount, 
					   0, 
					   idx_Size, 
					   idx_IndexBufferFormat, 
					   0, 
					   buffer);

	//idx.m_IndexBuffer = Engine::GetAPI()->GetDevice().CreateBuffer(idx_desc);
}

template<typename T>
void CModelImporter::FillVertexData(T* out, ModelData* data, Effect* effect)
{
	BaseModel* model = static_cast<BaseModel*>(out);
	auto& vtx = model->m_VertexWrapper;

	s32 sizeOfBuffer = data->myVertexCount * data->myVertexStride * sizeof(float);
	s8* vertexRawData = new s8[sizeOfBuffer];
	memcpy(vertexRawData, data->myVertexBuffer, sizeOfBuffer);

	const s8* vtx_Data = vertexRawData;
	const s32 vtx_VertexCount = data->myVertexCount;
	const s32 vtx_Size = sizeOfBuffer;
	const s32 vtx_Stride = data->myVertexStride * sizeof(float);
	const s32 vtx_start = 0;
	const s32 vtx_buff_count = 1;

	graphics::BufferDesc vtx_desc;
	vtx_desc.m_Size = vtx_Size;
	vtx_desc.m_Data = (void*)vtx_Data;
	vtx_desc.m_BindFlag = graphics::BIND_VERTEX_BUFFER;
	vtx_desc.m_UsageFlag = graphics::DYNAMIC_USAGE;
	vtx_desc.m_StructuredByteStride = 0;
	vtx_desc.m_CPUAccessFlag = graphics::WRITE;
	vtx_desc.m_MiscFlags = 0;
	vtx_desc.m_ByteWidth = vtx.m_Size;
	IBuffer* buffer = Engine::GetAPI()->GetDevice().CreateBuffer(vtx_desc);


	CU::GrowingArray<graphics::InputElementDesc> element;
	SetupInputLayout(data, element);

	IInputLayout* layout = Engine::GetAPI()->GetDevice().CreateInputLayout(effect->GetVertexShader(), &element[0], element.Size());
	vtx = VertexWrapper(vertexRawData, vtx_start, 1, vtx_Stride, 0, vtx_VertexCount, vtx_Size, buffer, layout, graphics::TRIANGLE_LIST);
}

template<typename T>
void CModelImporter::FillInstanceData(T* out, ModelData* data, Effect* effect)
{
	BaseModel* model = static_cast<BaseModel*>(out);
	auto& ins = model->m_InstanceWrapper;
	const s32 ins_BufferCount = 1;
	const s32 ins_Start = 0;
	const s32 ins_Stride = sizeof(CU::Matrix44f);
	const s32 ins_ByteOffset = 0;
	const s32 ins_InstanceCount = 5000;
	const s32 ins_Size = ins_InstanceCount * ins_Stride;
	const s32 ins_IndicesPerInstance = model->m_IndexWrapper.GetIndexCount();

	graphics::BufferDesc desc;
	desc.m_BindFlag = graphics::BIND_VERTEX_BUFFER;
	desc.m_UsageFlag = graphics::DYNAMIC_USAGE;
	desc.m_CPUAccessFlag = graphics::WRITE;
	desc.m_ByteWidth = ins_Size;
	


	IBuffer* buffer = Engine::GetAPI()->GetDevice().CreateBuffer(desc);

	CU::GrowingArray<graphics::InputElementDesc> element;
	SetupInputLayout(data, element);

	s32 byte_offset = 0;
	graphics::InputElementDesc instance[4] = {
		{ "INSTANCE", 0, graphics::_16BYTE_RGBA, 1, 0, graphics::INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 1, graphics::_16BYTE_RGBA, 1, 16, graphics::INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 2, graphics::_16BYTE_RGBA, 1, 32, graphics::INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 3, graphics::_16BYTE_RGBA, 1, 48, graphics::INPUT_PER_INSTANCE_DATA, 1 }
	};

	element.Add(instance[0]);
	element.Add(instance[1]);
	element.Add(instance[2]);
	element.Add(instance[3]);

	IInputLayout* layout = Engine::GetAPI()->GetDevice().CreateInputLayout(effect->GetVertexShader(), &element[0], element.Size());

	ins = InstanceWrapper(ins_InstanceCount, ins_IndicesPerInstance, ins_ByteOffset, ins_Stride, ins_Start, ins_BufferCount, buffer, layout);

}

