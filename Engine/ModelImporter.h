#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Engine/Surface.h>
#include "engine_shared.h"

#ifdef _DEBUG
#include <CommonLib/Timer/TimeManager.h>
#endif

#include <Engine/Model.h>
#include <Engine/LightModel.h>
#include <Engine/AtmosphereModel.h>


#include <Engine/VertexWrapper.h>
#include <Engine/IndexWrapper.h>

#include <Engine/Effect.h>

#include <iostream>

#include <Engine/IGraphicsAPI.h>
#include <Engine/IGraphicsDevice.h>
#include <Engine/IGraphicsContext.h>

#include <Engine/ModelImporterFlags.h>

//#define IMPORT_THREAD
#ifdef IMPORT_THREAD
#include <CommonLib/Threadpool.h>
#endif
#include "ModelExporter.h"
#include "shader_types.h"

#include <CommonLib/File.h>


class Engine;
class aiNode;
class aiMesh;
class aiScene;


constexpr int32 TRIANGLE_VERTEX_COUNT = 3;
constexpr int32 VERTEX_STRIDE = 4;
constexpr int32 NORMAL_STRIDE = 4;
constexpr int32 BINORMAL_STRIDE = 4;
constexpr int32 TANGENT_STRIDE = 4;
constexpr int32 SKINWEIGHT_STRIDE = 4;
constexpr int32 BONEID_STRIDE = 4;
constexpr int32 UV_STRIDE = 2;


struct VertexData
{
	CU::Vector4f m_Position;
	CU::Vector4f m_Normal;
	CU::Vector2f m_UV;
	CU::Vector4f m_BiNormal;
	CU::Vector4f m_Tangent;

	friend std::ostream & operator<<(std::ostream& out, const VertexData& data)
	{
		out << data.m_Position.x << " ";
		out << data.m_Position.y << " ";
		out << data.m_Position.z << " ";
		out << data.m_Position.w << " ";

		out << data.m_Normal.x << " ";
		out << data.m_Normal.y << " ";
		out << data.m_Normal.z << " ";
		out << data.m_Normal.w << " ";

		out << data.m_UV.x << " ";
		out << data.m_UV.y << " ";

		out << data.m_BiNormal.x << " ";
		out << data.m_BiNormal.y << " ";
		out << data.m_BiNormal.z << " ";
		out << data.m_BiNormal.w << " ";

		out << data.m_Tangent.x << " ";
		out << data.m_Tangent.y << " ";
		out << data.m_Tangent.z << " ";
		out << data.m_Tangent.w << " ";
		return out;
	}
};

class CModelImporter
{
public:
	CModelImporter() = default;
	~CModelImporter() = default;

	enum ImportOptions
	{
		NONE = 0,
		SKIP_FILL = 1,
	};

	template<typename T>
	void LoadModel(std::string filepath, T* pModel, Effect* effect, int options = 0);

	struct TextureInfo //Anyway to remove this?
	{
		TextureSlot m_Slot;
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
			int32 mySize;
			int32 myOffset;
			LayoutType myType;
		};

		float* myVertexBuffer = nullptr;
		int32* myIndicies = nullptr;
		uint32 myVertexCount = 0;
		uint32 m_VertexBufferSize = 0;
		uint32 m_IndexBufferSize = 0;
		uint32 myVertexStride = 0; //byte distance between each vertex
		uint32 myIndexCount = 0;
		CU::GrowingArray<TextureInfo> myTextures;
		CU::GrowingArray<Layout> myLayout;

		CU::Vector3f m_MinPoint;
		CU::Vector3f m_MaxPoint;
		std::string m_Filename;

	};
private:

#ifdef IMPORT_THREAD
	Threadpool m_Pool;
#endif
	Effect* m_Effect;

	int m_Options = 0;

	template<typename T>
	void FillData(const ModelData& data, T* model, std::string filepath);

	template<typename T>
	void FillInstanceData(T* out, const ModelData& data, Effect* effect);

	template<typename T>
	void FillIndexData(T* out, const ModelData& data);
	template<typename T>
	void FillVertexData(T* out, const ModelData& data, Effect* effect);

	void SetupInputLayout(const ModelData& data, CU::GrowingArray<graphics::InputElementDesc>& element_desc);

	template <typename T>
	void ProcessNode(aiNode* aNode, const aiScene* scene, std::string file, T* parent);

	template<typename T>
	void ProcessMesh(unsigned int index, const aiScene* scene, std::string file, T* parent);

	void AddTangentData(uint32 &stride, ModelData &data, uint32 &size, uint32 polygonVertexCount);

	void AddBiNormalData(uint32 &stride, ModelData &data, uint32 &size, uint32 polygonVertexCount);



	void ExtractMaterials(aiMesh* mesh, const aiScene* scene, ModelData& data, std::string file);

	template <typename T>
	void Read(std::string path, T* pModel);

	template <typename T>
	void ReadBlock(const char* data, uint32& position, T* pModel);

	template <typename T>
	void ReadData(const char* data, uint32& position, T& out);

	void AddBoneData(uint32& stride, ModelData& data, uint32& size, uint32 polygonVertexCount);

	void AddVertexPositionData(uint32 &stride, ModelData& data, uint32 &size, uint32 polygonVertexCount);

	void AddVertexNormalData(uint32 &stride, ModelData &data, uint32 &size, uint32 polygonVertexCount);

	void AddUVData(uint32 &stride, ModelData &data, uint32 &size, uint32 polygonVertexCount);

};

static bool instanced = false;

template<typename T>
void CModelImporter::LoadModel(std::string filepath, T* pModel, Effect* effect, int options)
{
	CU::TimeManager timer;
	m_Options = options;
	instanced = false;
	DL_MESSAGE("Loading model : %s", filepath.c_str());
	m_Effect = effect;

	if (filepath.find("EMF") != filepath.npos)
	{
		pModel->m_FileName = filepath;
		Read(filepath, pModel);
		pModel->SetIsInstanced(instanced);
		return;
	}



	timer.Update();
	float loadTime = timer.GetMasterTimer().GetTotalTime().GetMilliseconds();
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
#ifdef IMPORT_THREAD
	m_Pool.Initiate("model-importer");
#endif
	//This code should be moved to release and kept running at release for fast load times in debug.
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, processFlags);
	timer.Update();
	float read_time = timer.GetMasterTimer().GetTotalTime().GetMilliseconds() - loadTime;
	DL_MESSAGE("%s took %.1fms to read.", filepath.c_str(), read_time);

	DL_MESSAGE_EXP(!scene, "%s", importer.GetErrorString());
	//ASSERT(scene, "ImportModel Failed. Could not read the requested file.");
	if (!scene)
	{

		return;
	}
	aiNode* rootNode = scene->mRootNode;

	ProcessNode(rootNode, scene, filepath, pModel);



#ifdef IMPORT_THREAD
	m_Pool.CleanUp();
#endif

	pModel->SetIsInstanced(instanced);

	timer.Update();

	float _load_time = timer.GetMasterTimer().GetTotalTime().GetMilliseconds() - read_time;
	DL_MESSAGE("%s took %.1fms to load.", filepath.c_str(), _load_time);

	loadTime = timer.GetMasterTimer().GetTotalTime().GetMilliseconds() - loadTime;
	DL_MESSAGE("%s took a total of %.1fms to load.", filepath.c_str(), loadTime);


	size_t pos = filepath.rfind("/");
	if (pos == std::string::npos)
		pos = filepath.rfind("\\");

	size_t end = filepath.rfind(".");
	end = end - pos;
	std::string file = filepath.substr(pos + 1, end - 1);



	char temp[256];
	sprintf_s(temp, "data/exported/%s", file.c_str());
	ModelExporter exporter;
	exporter.Export(pModel, temp);

}

template<typename T>
void CModelImporter::FillData(const ModelData& data, T* out, std::string filepath)
{
	out->SetEffect(m_Effect);
	//data.m_Filename = filepath;
	size_t pos = filepath.rfind('/');
	std::string path = filepath.substr(0, pos + 1);
	FillVertexData(out, data, m_Effect);
	FillIndexData(out, data);

	auto& vtx = out->m_VertexWrapper;

	if (!(m_Options&ModelImportUtil::IGNORE_FILL))
	{
		if (!vtx.GetInputLayout())
			FillInstanceData(out, data, m_Effect);

		if (filepath.find("cube_100x100") != filepath.npos)
			return;

		const bool custom_format = filepath.find("EMF") != std::string::npos;

		Surface* surface = new Surface(m_Effect);
		for (auto& tex : data.myTextures)
		{

			std::string new_path;
			if (!custom_format)
				new_path += path;

			surface->AddTexture(new_path + tex.m_File, tex.m_Slot);
		}
		out->AddSurface(surface);

	}

}

inline void CModelImporter::SetupInputLayout(const ModelData& data, CU::GrowingArray<graphics::InputElementDesc>& element_desc)
{
	for (int i = 0; i < data.myLayout.Size(); ++i)
	{
		auto currentLayout = data.myLayout[i];
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
			desc.m_Semantic = "WEIGHTS";
			desc.m_Format = graphics::_16BYTE_RGBA;
		}
		else if (currentLayout.myType == ModelData::VERTEX_BONEID)
		{
			desc.m_Semantic = "BONES";
			desc.m_Format = graphics::_16BYTE_RGBA;
		}
		element_desc.Add(desc);
	}
}

template <typename T>
void CModelImporter::ProcessNode(aiNode* aNode, const aiScene* scene, std::string file, T* parent)
{
#ifdef IMPORT_THREAD
	bool _thread = false;
	bool thread2 = false;
#endif
	volatile int32 _meshes_done = 0;
	for (uint32 i = 0; i < aNode->mNumMeshes; i++)
	{
#ifdef IMPORT_THREAD
		if (_thread)
		{
			m_Pool.AddWork(Work([&, i]
			{
				unsigned int index = aNode->mMeshes[i];
				ProcessMesh<T>(index, scene, file, parent);
				_meshes_done++;
			}));
		}
		else
#endif
		{
			ProcessMesh<T>(aNode->mMeshes[i], scene, file, parent);
			_meshes_done++;
		}
	}

#ifdef IMPORT_THREAD
	while (_meshes_done < aNode->mNumMeshes)
	{
		m_Pool.Update();
	}
#endif

	for (uint32 i = 0; i < aNode->mNumChildren; i++)
	{
		aiNode* node = aNode->mChildren[i];
#ifdef IMPORT_THREAD
		if (thread2)
		{
			m_Pool.AddWork(Work([=] { ProcessNode<T>(node, scene, file, parent); }));
		}
		else
#endif
		{
			ProcessNode<T>(node, scene, file, parent);
		}
	}
}

template<typename T>
void CModelImporter::ProcessMesh(unsigned int index, const aiScene* scene, std::string file, T* parent)
{
	aiMesh* mesh = scene->mMeshes[index];

	ModelData* data = new ModelData;

	uint32 polygonCount = mesh->mNumFaces;
	uint32 size = polygonCount * VERTEX_STRIDE;
	uint32 polygonVertexCount = polygonCount * 4;
	uint32 stride = 0;

	if (mesh->HasPositions())
		AddVertexPositionData(stride, *data, size, polygonVertexCount);

	if (mesh->HasNormals())
		AddVertexNormalData(stride, *data, size, polygonVertexCount);

	if (mesh->HasTextureCoords(0)) //this is multiple coordinates 
		AddUVData(stride, *data, size, polygonVertexCount);

	if (mesh->HasTangentsAndBitangents())
	{
		AddBiNormalData(stride, *data, size, polygonVertexCount);
		AddTangentData(stride, *data, size, polygonVertexCount);
	}

	if (mesh->HasBones())
		AddBoneData(stride, *data, size, polygonVertexCount);

	//DL_MESSAGE("Vertex Buffer Array Size : %d", size);

	const uint32 vtx_size = size;


	data->myVertexBuffer = new float[vtx_size];
	ZeroMemory(data->myVertexBuffer, sizeof(float) * vtx_size);
	data->m_VertexBufferSize = sizeof(float) * vtx_size;
	ASSERT(mesh->mNumVertices < size, "the amount of vertices was MORE!? than size");

	const uint32 index_count = polygonCount * 3;
	CU::GrowingArray<uint32> indices(index_count);

	uint32 vertCount = 0;

	for (uint32 i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace* face = &mesh->mFaces[i];

		for (int32 j = 2; j >= 0; j--)
		{
			uint32 addedSize = VERTEX_STRIDE;
			uint32 currIndex = vertCount * stride;
			ASSERT(addedSize <= size, "addedSize was larger than the size of the array.");
			uint32 verticeIndex = face->mIndices[j];

			if (mesh->HasPositions())
			{
				CU::Vector4f position(
					mesh->mVertices[verticeIndex].x,
					mesh->mVertices[verticeIndex].y,
					mesh->mVertices[verticeIndex].z,
					1);

				if (m_Options & ModelImportUtil::FLIP_VERTEX)
				{
					CU::Matrix44f fixMatrix = CU::Math::CreateReflectionMatrixAboutAxis44(CU::Vector3f(1, 0, 0));
					position = position * fixMatrix;
				}

				data->myVertexBuffer[currIndex] = position.x;
				data->myVertexBuffer[currIndex + 1] = position.y;
				data->myVertexBuffer[currIndex + 2] = position.z;
				data->myVertexBuffer[currIndex + 3] = 1;


				if (mesh->HasNormals())
				{

					CU::Vector3f normal(
						mesh->mNormals[verticeIndex].x,
						mesh->mNormals[verticeIndex].y,
						mesh->mNormals[verticeIndex].z);

					if (m_Options & ModelImportUtil::FLIP_NORMAL)
					{
						normal = normal * CU::Math::CreateReflectionMatrixAboutAxis(CU::Vector3f(1, 0, 0));
						CU::Math::Normalize(normal);
					}

					data->myVertexBuffer[currIndex + addedSize] = normal.x;
					data->myVertexBuffer[currIndex + addedSize + 1] = normal.y;
					data->myVertexBuffer[currIndex + addedSize + 2] = normal.z;
					data->myVertexBuffer[currIndex + addedSize + 3] = 0;
					addedSize += NORMAL_STRIDE;
				}

				if (mesh->HasTextureCoords(0))
				{
					CU::Vector2f uv(mesh->mTextureCoords[0][verticeIndex].x, mesh->mTextureCoords[0][verticeIndex].y * -1.f);


					data->myVertexBuffer[currIndex + addedSize] = uv.x;
					data->myVertexBuffer[currIndex + addedSize + 1] = uv.y;
					addedSize += UV_STRIDE;
				}

				if (mesh->HasTangentsAndBitangents())
				{

					CU::Vector3f binorm(
						mesh->mBitangents[verticeIndex].x,
						mesh->mBitangents[verticeIndex].y,
						mesh->mBitangents[verticeIndex].z);

					if (m_Options & ModelImportUtil::FLIP_BINORM)
					{
						binorm = binorm * CU::Math::CreateReflectionMatrixAboutAxis(CU::Vector3f(1, 0, 0));
						CU::Math::Normalize(binorm);
					}

					data->myVertexBuffer[currIndex + addedSize] = binorm.x;
					data->myVertexBuffer[currIndex + addedSize + 1] = binorm.y;
					data->myVertexBuffer[currIndex + addedSize + 2] = binorm.z;
					data->myVertexBuffer[currIndex + addedSize + 3] = 0;
					addedSize += BINORMAL_STRIDE;

					CU::Vector3f tangent(
						mesh->mTangents[verticeIndex].x,
						mesh->mTangents[verticeIndex].y,
						mesh->mTangents[verticeIndex].z);

					if (m_Options & ModelImportUtil::FLIP_TANGENT)
					{
						tangent = tangent * CU::Math::CreateReflectionMatrixAboutAxis(CU::Vector3f(-1, 0, 0));
						CU::Math::Normalize(tangent);
					}

					data->myVertexBuffer[currIndex + addedSize] = tangent.x;
					data->myVertexBuffer[currIndex + addedSize + 1] = tangent.y;
					data->myVertexBuffer[currIndex + addedSize + 2] = tangent.z;
					data->myVertexBuffer[currIndex + addedSize + 3] = 0;
					addedSize += TANGENT_STRIDE;
				}

				indices.Add(verticeIndex);
				//data.myIndicies[idx_count] = verticeIndex;
				vertCount++;
			}
		}

	}

	data->myVertexStride = stride * sizeof(float);
	data->myVertexCount = vertCount;
	data->myIndexCount = indices.Size();


	data->myIndicies = new int32[index_count];
	ZeroMemory(data->myIndicies, sizeof(int32) * index_count);
	data->m_IndexBufferSize = index_count * sizeof(int32);

	memcpy(data->myIndicies, &indices[0], indices.Size() * sizeof(int32));



	ExtractMaterials(mesh, scene, *data, mesh->mName.C_Str());

	T* child = new T;
	child->m_IsRoot = false;
	FillData(*data, child, file);

	static Ticket_Mutex add_mutex;
	BeginTicketMutex(&add_mutex);
	parent->AddChild(child);
	EndTicketMutex(&add_mutex);

	//delete data;


}

template<typename T>
void CModelImporter::FillIndexData(T* out, const ModelData& data)
{
	auto& idx = out->m_IndexWrapper;
	idx.Reset();

	const int32 idx_buf_size = data.m_IndexBufferSize;

	// 	s8* indexData = new s8[idx_buf_size];
	// 	memcpy(indexData, &data->myIndicies[0], idx_buf_size);

	const graphics::eTextureFormat idx_IndexBufferFormat = graphics::R32_UINT;
	const int32 idx_IndexCount = data.myIndexCount;
	const int32 idx_Size = idx_IndexCount * sizeof(uint32);

	graphics::BufferDesc idx_desc;
	idx_desc.m_Size = idx_buf_size;
	idx_desc.m_Data = (int8*)data.myIndicies;
	idx_desc.m_BindFlag = graphics::BIND_INDEX_BUFFER;
	idx_desc.m_UsageFlag = graphics::IMMUTABLE_USAGE;
	idx_desc.m_StructuredByteStride = 0;
	idx_desc.m_CPUAccessFlag = graphics::NO_ACCESS_FLAG;
	idx_desc.m_MiscFlags = 0;
	idx_desc.m_ByteWidth = idx_desc.m_Size;

	idx.SetData((int8*)data.myIndicies);
	idx.SetIndexCount(idx_IndexCount);
	idx.SetStart(0);
	idx.SetSize(idx_buf_size);
	idx.SetFormat(idx_IndexBufferFormat);
	idx.SetByteOffset(0);

	if (!(m_Options & ModelImportUtil::IGNORE_FILL))
	{
		graphics::IGraphicsDevice* device = &Engine::GetAPI()->GetDevice();
		IBuffer* buffer = device->CreateBuffer(idx_desc, data.m_Filename + " IndexBuffer");
		idx.SetBuffer(buffer);
	}
#ifdef _DEBUG
	idx.m_DebugName = DEBUG_NAME_A(data.m_Filename, T);
#endif
}

template<typename T>
void CModelImporter::FillVertexData(T* out, const ModelData& data, Effect* effect)
{
	auto& vtx = out->m_VertexWrapper;
	vtx.Reset();

	const int32 vtx_VertexCount = data.myVertexCount;
	const int32 vtx_Stride = data.myVertexStride;
	const int32 vtx_start = 0;
	const int32 vtx_buff_count = 1;
	const int32 vtx_Size = data.m_VertexBufferSize;

	//DL_MESSAGE("Buffer Size : %d", vtx_VertexCount);

	graphics::BufferDesc vtx_desc;
	vtx_desc.m_Size = vtx_VertexCount * vtx_Stride;
	vtx_desc.m_Data = (int8*)data.myVertexBuffer;
	vtx_desc.m_BindFlag = graphics::BIND_VERTEX_BUFFER;
	vtx_desc.m_UsageFlag = graphics::DYNAMIC_USAGE;
	vtx_desc.m_CPUAccessFlag = graphics::WRITE;
	vtx_desc.m_ByteWidth = vtx_desc.m_Size;
	vtx_desc.m_StructuredByteStride = 0;
	vtx_desc.m_MiscFlags = 0;

	if (!(m_Options & ModelImportUtil::IGNORE_FILL))
	{
		graphics::IGraphicsDevice* device = &Engine::GetAPI()->GetDevice();
		IBuffer* buffer = device->CreateBuffer(vtx_desc, data.m_Filename + "VertexBuffer");

		CU::GrowingArray<graphics::InputElementDesc> element;
		SetupInputLayout(data, element);


		if (!device->FindInputSemantic("INSTANCE", effect->GetVertexShader()->m_Blob))
		{
			IInputLayout* layout = device->CreateInputLayout(effect->GetVertexShader(), &element[0], element.Size());
			vtx.SetInputLayout(layout);
		}

		vtx.SetBuffer(buffer);
	}

	vtx.SetData((int8*)data.myVertexBuffer);
	vtx.SetStart(vtx_start);
	vtx.SetStride(vtx_Stride);
	vtx.SetByteOffset(0);
	vtx.SetVertexCount(vtx_VertexCount);
	vtx.SetSize(vtx_Size);


	vtx.SetTopology(graphics::TRIANGLE_LIST);
#ifdef _DEBUG
	vtx.m_DebugName = DEBUG_NAME_A(data.m_Filename, T);
#endif

}

template<typename T>
void CModelImporter::FillInstanceData(T* out, const ModelData& data, Effect* effect)
{
	auto& ins = out->m_InstanceWrapper;
	ins.Reset();

	const int32 ins_BufferCount = 1;
	const int32 ins_Start = 0;
	const int32 ins_ByteOffset = 0;
	const int32 ins_InstanceCount = 500;
	const int32 ins_IndicesPerInstance = data.myIndexCount;

	/*
		There's an optimization right here.

		This should not be created for every single instance wrapper.

		Only if it's a root model.


	*/
	//const s32 ins_Stride = sizeof(GPUModelData);
	//const s32 ins_Size = ins_InstanceCount * ins_Stride;

	/*graphics::BufferDesc desc;
	desc.m_BindFlag = graphics::BIND_VERTEX_BUFFER;
	desc.m_UsageFlag = graphics::DYNAMIC_USAGE;
	desc.m_CPUAccessFlag = graphics::WRITE;
	desc.m_ByteWidth = ins_Size;

	IBuffer* buffer = m_Device->CreateBuffer(desc, data.m_Filename + "InstanceBuffer");*/

	CU::GrowingArray<graphics::InputElementDesc> element;
	SetupInputLayout(data, element);

	graphics::SInputLayout _layout;
	_layout.m_Elements = element;
	_layout.AddElement("INSTANCE", graphics::_16BYTE_RGBA, 1, true, 0);
	_layout.AddElement("INSTANCE", graphics::_16BYTE_RGBA, 1, true, 1);
	_layout.AddElement("INSTANCE", graphics::_16BYTE_RGBA, 1, true, 2);
	_layout.AddElement("INSTANCE", graphics::_16BYTE_RGBA, 1, true, 3);

	_layout.AddElement("DATA", graphics::_16BYTE_RGBA, 1, true, 0);
#ifdef _DEBUG
	_layout.AddElement("ID", graphics::_4BYTE_R_UINT, 1, true, 0);
#endif

	IInputLayout* layout = Engine::GetAPI()->GetDevice().CreateInputLayout(effect->GetVertexShader(), _layout);

	ins.SetBuffer(nullptr);
	ins.SetInputLayout(layout);
	ins.SetIndexCountPerInstance(ins_IndicesPerInstance);
	ins.SetInstanceCount(ins_InstanceCount);
	ins.SetByteOffset(ins_ByteOffset);
	ins.SetStride(sizeof(GPUModelData));
	ins.SetBufferCount(ins_BufferCount);
	instanced = true;

	//out->SetIsInstanced(true);

#ifdef _DEBUG
	ins.m_DebugName = DEBUG_NAME_A(data.m_Filename, T);
#endif

}

#undef IMPORT_THREAD

template <typename T>
void CModelImporter::Read(std::string path, T* pModel)
{
	pModel->m_FileName = path;
	Core::File file(path.c_str());
	uint32 position = 0;
	EMFHeader header = {};


	memcpy(&header, &file.GetBuffer()[0], sizeof(EMFHeader));
	position += sizeof(EMFHeader);

	ReadBlock(file.GetBuffer(), position, pModel);
}

template <typename T>
void CModelImporter::ReadBlock(const char* data, uint32& position, T* pModel)
{



	ModelData model_data; // = new ModelData;

	uint32 stride = 0;
	ModelData::Layout newLayout;
	//if (header.hasVertices)
	{
		newLayout.myType = ModelData::VERTEX_POS;
		newLayout.mySize = VERTEX_STRIDE;
		newLayout.myOffset = 0;
		model_data.myLayout.Add(newLayout);
		stride += VERTEX_STRIDE;
	}

	//if (header.hasNormals)
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_NORMAL;
		newLayout.mySize = NORMAL_STRIDE;
		newLayout.myOffset = stride * 4;
		model_data.myLayout.Add(newLayout);

		stride += NORMAL_STRIDE;
	}

	//if (header.hasTexcoord) 
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_UV;
		newLayout.mySize = UV_STRIDE;
		newLayout.myOffset = stride * 4;
		model_data.myLayout.Add(newLayout);

		stride += UV_STRIDE;
	}

	//if (header.hasBiNormals)
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_BINORMAL;
		newLayout.mySize = BINORMAL_STRIDE;
		newLayout.myOffset = stride * 4;
		model_data.myLayout.Add(newLayout);

		stride += BINORMAL_STRIDE;
	}

	//if(header.hasTangents)
	{
		newLayout.myType = ModelData::VERTEX_TANGENT;
		newLayout.mySize = TANGENT_STRIDE;
		newLayout.myOffset = stride * 4;
		model_data.myLayout.Add(newLayout);

		stride += TANGENT_STRIDE;
	}


	model_data.myVertexStride = stride * sizeof(float);

	ReadData(data, position, model_data.myVertexCount);

	if (model_data.myVertexCount > 0)
	{
		model_data.m_VertexBufferSize = model_data.myVertexCount * sizeof(VertexData);
		int8* buffer = new int8[model_data.m_VertexBufferSize];

		ZeroMemory(&buffer[0], model_data.m_VertexBufferSize);
		memcpy(&buffer[0], &data[position], model_data.m_VertexBufferSize);

		model_data.myVertexBuffer = (float*)buffer;
		position += model_data.m_VertexBufferSize;
	}

	ReadData(data, position, model_data.myIndexCount);
	if (model_data.myIndexCount > 0)
	{
		model_data.m_IndexBufferSize = model_data.myIndexCount * sizeof(int);
		int8* buffer = new int8[model_data.m_IndexBufferSize];

		ZeroMemory(&buffer[0], model_data.m_IndexBufferSize);
		memcpy(&buffer[0], &data[position], model_data.m_IndexBufferSize);

		model_data.myIndicies = (int*)buffer;

		position += model_data.m_IndexBufferSize;
	}

	int surface_count = 0;
	ReadData(data, position, surface_count);
	if (surface_count > 0)
	{
		int nof_bindings = 0;
		ReadData(data, position, nof_bindings);
		for (int i = 0; i < nof_bindings; ++i)
		{

			TextureInfo info;

			int length = 0;
			ReadData(data, position, length);

			char* resource_name = new char[length + 1];
			ZeroMemory(&resource_name[0], length);
			resource_name[length] = '\0';
			memcpy(&resource_name[0], &data[position], sizeof(char) * length);
			info.m_File = resource_name;

			position += sizeof(char) * length;


			int slot = 0;
			ReadData(data, position, slot);

			info.m_Slot = static_cast<TextureSlot>(slot);

			model_data.myTextures.Add(info);

		}
	}

	if (model_data.myVertexCount > 0)
		FillData(model_data, pModel, pModel->m_FileName);

	int child_count = 0;
	ReadData(data, position, child_count);
	for (int i = 0; i < child_count; ++i)
	{

		T* child = new T;
		child->m_IsRoot = false;
		child->m_FileName = pModel->m_FileName;
		pModel->AddChild(child);
		ReadBlock(data, position, child);
	}

}

template <typename T>
void CModelImporter::ReadData(const char* data, uint32& position, T& out)
{
	memcpy(&out, &data[position], sizeof(T));
	position += sizeof(T);
}