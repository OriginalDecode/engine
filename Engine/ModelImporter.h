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


#include <Engine/IGraphicsAPI.h>
#include <Engine/IGraphicsDevice.h>
#include <Engine/IGraphicsContext.h>

//#define IMPORT_THREAD
#ifdef IMPORT_THREAD
#include <CommonLib/Threadpool.h>
#endif
#include "ModelExporter.h"
#include "shader_types.h"
class Engine;
class aiNode;
class aiMesh;
class aiScene;


constexpr s32 TRIANGLE_VERTEX_COUNT = 3;
constexpr s32 VERTEX_STRIDE = 4;
constexpr s32 NORMAL_STRIDE = 4;
constexpr s32 BINORMAL_STRIDE = 4;
constexpr s32 TANGENT_STRIDE = 4;
constexpr s32 SKINWEIGHT_STRIDE = 4;
constexpr s32 BONEID_STRIDE = 4;
constexpr s32 UV_STRIDE = 2;


class CModelImporter
{
public:
	CModelImporter();


	template<typename T>
	void LoadModel(std::string filepath, T* pModel, Effect* effect);

private:
	Engine* m_Engine = nullptr;
#pragma region Structs
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
			s32 mySize;
			s32 myOffset;
			LayoutType myType;
		};

		float* myVertexBuffer = nullptr;
		s32* myIndicies = nullptr;
		u32 myVertexCount = 0;
		u32 m_VertexBufferSize = 0;
		u32 m_IndexBufferSize = 0;
		u32 myVertexStride = 0; //byte distance between each vertex
		u32 myIndexCount = 0;
		CU::GrowingArray<TextureInfo> myTextures;
		CU::GrowingArray<Layout> myLayout;

		CU::Vector3f m_MinPoint;
		CU::Vector3f m_MaxPoint;
		std::string m_Filename;

	};
#pragma endregion

#ifdef IMPORT_THREAD
	Threadpool m_Pool;
#endif
	Effect* m_Effect;

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

	void ExtractMaterials(aiMesh* mesh, const aiScene* scene, ModelData& data, std::string file);

	template <typename T>
	void Read(std::string path, T* pModel);

	template <typename T>
	void ReadBlock(const char* data, u32& position, T* pModel);

	template <typename T>
	void ReadData(const char* data, u32& position, T& out);



};

static bool instanced = false;

template<typename T>
void CModelImporter::LoadModel(std::string filepath, T* pModel, Effect* effect)
{
	CU::TimeManager timer;

	instanced = false;
	DL_MESSAGE("Loading model : %s", filepath.c_str());

	if (filepath.find("LPMF") != filepath.npos)
	{
		Read(filepath, pModel);
		return;
	}



	m_Effect = effect;
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
	ASSERT(scene, "ImportModel Failed. Could not read the requested file.");

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

	ModelExporter exporter;

	size_t pos = filepath.rfind("/");
	if (pos == std::string::npos)
		pos = filepath.rfind("\\");

	size_t end = filepath.rfind(".");
	end = end - pos;
	std::string file = filepath.substr(pos + 1, end - 1);



	char temp[256];
	sprintf_s(temp, "data/exported/%s", file.c_str());
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

	if (!vtx.GetInputLayout())
		FillInstanceData(out, data, m_Effect);

	if (filepath.find("cube_100x100") != filepath.npos)
		return;

	Surface* surface = new Surface(m_Effect);
	for (auto& tex : data.myTextures)
	{
		surface->AddTexture(path + tex.m_File, tex.m_Slot);
	}
	out->AddSurface(surface);


	return;
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


	bool _thread = false;
	bool thread2 = false;
	volatile s32 _meshes_done = 0;
	for (u32 i = 0; i < aNode->mNumMeshes; i++)
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

	for (u32 i = 0; i < aNode->mNumChildren; i++)
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
	
	ModelData data; // = new ModelData;

	u32 polygonCount = mesh->mNumFaces;
	u32 size = polygonCount * VERTEX_STRIDE;
	u32 polygonVertexCount = polygonCount * 4;

	u32 stride = 0;
	if (mesh->HasPositions())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_POS;
		newLayout.mySize = VERTEX_STRIDE;
		newLayout.myOffset = 0;
		data.myLayout.Add(newLayout);
		size += polygonVertexCount * VERTEX_STRIDE;
		stride += VERTEX_STRIDE;
	}

	if (mesh->HasNormals())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_NORMAL;
		newLayout.mySize = NORMAL_STRIDE;
		newLayout.myOffset = stride * 4;
		data.myLayout.Add(newLayout);

		stride += NORMAL_STRIDE;
		size += polygonVertexCount * NORMAL_STRIDE;
	}

	if (mesh->HasTextureCoords(0)) //this is multiple coords 
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_UV;
		newLayout.mySize = UV_STRIDE;
		newLayout.myOffset = stride * 4;
		data.myLayout.Add(newLayout);

		stride += UV_STRIDE;
		size += polygonVertexCount * UV_STRIDE;
	}

	if (mesh->HasTangentsAndBitangents())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_BINORMAL;
		newLayout.mySize = BINORMAL_STRIDE;
		newLayout.myOffset = stride * 4;
		data.myLayout.Add(newLayout);

		stride += BINORMAL_STRIDE;
		size += polygonVertexCount * BINORMAL_STRIDE;

		newLayout.myType = ModelData::VERTEX_TANGENT;
		newLayout.mySize = TANGENT_STRIDE;
		newLayout.myOffset = stride * 4;
		data.myLayout.Add(newLayout);

		stride += TANGENT_STRIDE;
		size += polygonVertexCount * TANGENT_STRIDE;
	}

	if (mesh->HasBones())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_SKINWEIGHTS;
		newLayout.mySize = SKINWEIGHT_STRIDE;
		newLayout.myOffset = stride * 4;
		data.myLayout.Add(newLayout);

		stride += SKINWEIGHT_STRIDE;
		size += polygonVertexCount * SKINWEIGHT_STRIDE;

		newLayout.myType = ModelData::VERTEX_BONEID;
		newLayout.mySize = BONEID_STRIDE;
		newLayout.myOffset = stride * 4;
		data.myLayout.Add(newLayout);

		stride += BONEID_STRIDE;
		size += polygonVertexCount * BONEID_STRIDE;
	}

	//DL_MESSAGE("Vertex Buffer Array Size : %d", size);

	const u32 vtx_size = size;


	data.myVertexBuffer = new float[vtx_size];
	ZeroMemory(data.myVertexBuffer, sizeof(float) * vtx_size);
	data.m_VertexBufferSize = sizeof(float) * vtx_size;
	ASSERT(mesh->mNumVertices < size, "the amount of vertices was MORE!? than size");

	const u32 index_count = polygonCount * 3;
	CU::GrowingArray<u32> indices(index_count);

	u32 vertCount = 0;
	
	for (u32 i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace* face = &mesh->mFaces[i];

		for (s32 j = 2; j >= 0; j--)
		{
			u32 addedSize = VERTEX_STRIDE;
			u32 currIndex = vertCount * stride;
			ASSERT(addedSize <= size, "addedSize was larger than the size of the array.");
			u32 verticeIndex = face->mIndices[j];

			if (mesh->HasPositions())
			{
				CU::Vector4f position(
					mesh->mVertices[verticeIndex].x,
					mesh->mVertices[verticeIndex].y,
					mesh->mVertices[verticeIndex].z,
					1);
				//CU::Matrix44f fixMatrix = CU::Math::CreateReflectionMatrixAboutAxis44(CU::Vector3f(1, 0, 0));
				//position = position * fixMatrix;

				data.myVertexBuffer[currIndex] = position.x;
				data.myVertexBuffer[currIndex + 1] = position.y;
				data.myVertexBuffer[currIndex + 2] = position.z;
				data.myVertexBuffer[currIndex + 3] = 1;


				if (mesh->HasNormals())
				{

					CU::Vector3f normal(
						mesh->mNormals[verticeIndex].x,
						mesh->mNormals[verticeIndex].y,
						mesh->mNormals[verticeIndex].z);
					//normal = normal * CU::Math::CreateReflectionMatrixAboutAxis(CU::Vector3f(1, 0, 0));
					//CU::Math::Normalize(normal);

					data.myVertexBuffer[currIndex + addedSize] = normal.x;
					data.myVertexBuffer[currIndex + addedSize + 1] = normal.y;
					data.myVertexBuffer[currIndex + addedSize + 2] = normal.z;
					data.myVertexBuffer[currIndex + addedSize + 3] = 0;
					addedSize += NORMAL_STRIDE;
				}

				if (mesh->HasTextureCoords(0))
				{
					CU::Vector2f uv(mesh->mTextureCoords[0][verticeIndex].x, mesh->mTextureCoords[0][verticeIndex].y * -1.f);


					data.myVertexBuffer[currIndex + addedSize] = uv.x;
					data.myVertexBuffer[currIndex + addedSize + 1] = uv.y;
					addedSize += UV_STRIDE;
				}

				if (mesh->HasTangentsAndBitangents())
				{

					CU::Vector3f binorm(
						mesh->mBitangents[verticeIndex].x,
						mesh->mBitangents[verticeIndex].y,
						mesh->mBitangents[verticeIndex].z);
					//binorm = binorm * CU::Math::CreateReflectionMatrixAboutAxis(CU::Vector3f(1, 0, 0));
					//CU::Math::Normalize(binorm);
					
					data.myVertexBuffer[currIndex + addedSize] = binorm.x;
					data.myVertexBuffer[currIndex + addedSize + 1] = binorm.y;
					data.myVertexBuffer[currIndex + addedSize + 2] = binorm.z;
					data.myVertexBuffer[currIndex + addedSize + 3] = 0;
					addedSize += BINORMAL_STRIDE;

					CU::Vector3f tangent(
						mesh->mTangents[verticeIndex].x,
						mesh->mTangents[verticeIndex].y,
						mesh->mTangents[verticeIndex].z);
					//tangent = tangent * CU::Math::CreateReflectionMatrixAboutAxis(CU::Vector3f(-1, 0, 0));
					//CU::Math::Normalize(tangent);

					data.myVertexBuffer[currIndex + addedSize] = tangent.x;
					data.myVertexBuffer[currIndex + addedSize + 1] = tangent.y;
					data.myVertexBuffer[currIndex + addedSize + 2] = tangent.z;
					data.myVertexBuffer[currIndex + addedSize + 3] = 0;
					addedSize += TANGENT_STRIDE;
				}

				indices.Add(verticeIndex);
				//data.myIndicies[idx_count] = verticeIndex;
				vertCount++;
			}
		}

	}

	data.myVertexStride = stride * sizeof(float);
	data.myVertexCount = vertCount;
	data.myIndexCount = indices.Size();


	data.myIndicies = new s32[index_count];
	ZeroMemory(data.myIndicies, sizeof(s32) * index_count);
	data.m_IndexBufferSize = index_count * sizeof(s32);
	
	memcpy(data.myIndicies, &indices[0], indices.Size() * sizeof(s32));



	ExtractMaterials(mesh, scene, data, mesh->mName.C_Str());

	T* child = new T;
	child->m_IsRoot = false;
	FillData(data, child, file);

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

	const s32 idx_buf_size = data.m_IndexBufferSize;

	// 	s8* indexData = new s8[idx_buf_size];
	// 	memcpy(indexData, &data->myIndicies[0], idx_buf_size);

	const graphics::eTextureFormat idx_IndexBufferFormat = graphics::R32_UINT;
	const s32 idx_IndexCount = data.myIndexCount;
	const s32 idx_Size = idx_IndexCount * sizeof(u32);

	graphics::BufferDesc idx_desc;
	idx_desc.m_Size = idx_buf_size;
	idx_desc.m_Data = (s8*)data.myIndicies;
	idx_desc.m_BindFlag = graphics::BIND_INDEX_BUFFER;
	idx_desc.m_UsageFlag = graphics::IMMUTABLE_USAGE;
	idx_desc.m_StructuredByteStride = 0;
	idx_desc.m_CPUAccessFlag = graphics::NO_ACCESS_FLAG;
	idx_desc.m_MiscFlags = 0;
	idx_desc.m_ByteWidth = idx_desc.m_Size;

	IBuffer* buffer = Engine::GetAPI()->GetDevice().CreateBuffer(idx_desc, data.m_Filename + " IndexBuffer");

	idx.SetData((s8*)data.myIndicies);
	idx.SetIndexCount(idx_IndexCount);
	idx.SetStart(0);
	idx.SetSize(idx_buf_size);
	idx.SetFormat(idx_IndexBufferFormat);
	idx.SetByteOffset(0);
	idx.SetBuffer(buffer);
#ifdef _DEBUG
	idx.m_DebugName = DEBUG_NAME_A(data.m_Filename, T);
#endif
}

template<typename T>
void CModelImporter::FillVertexData(T* out, const ModelData& data, Effect* effect)
{
	auto& vtx = out->m_VertexWrapper;
	graphics::IGraphicsDevice& device = Engine::GetAPI()->GetDevice();

	const s32 vtx_VertexCount = data.myVertexCount;
	const s32 vtx_Stride = data.myVertexStride;
	const s32 vtx_start = 0;
	const s32 vtx_buff_count = 1;
	const s32 vtx_Size = data.m_VertexBufferSize;

	//DL_MESSAGE("Buffer Size : %d", vtx_VertexCount);

	graphics::BufferDesc vtx_desc;
	vtx_desc.m_Size = vtx_VertexCount * vtx_Stride;
	vtx_desc.m_Data = (s8*)data.myVertexBuffer;
	vtx_desc.m_BindFlag = graphics::BIND_VERTEX_BUFFER;
	vtx_desc.m_UsageFlag = graphics::DYNAMIC_USAGE;
	vtx_desc.m_CPUAccessFlag = graphics::WRITE;
	vtx_desc.m_ByteWidth = vtx_desc.m_Size;
	vtx_desc.m_StructuredByteStride = 0;
	vtx_desc.m_MiscFlags = 0;
	IBuffer* buffer = device.CreateBuffer(vtx_desc, data.m_Filename + "VertexBuffer");


	CU::GrowingArray<graphics::InputElementDesc> element;
	SetupInputLayout(data, element);


	if (!device.FindInputSemantic("INSTANCE", effect->GetVertexShader()->m_Blob))
	{
		IInputLayout* layout = device.CreateInputLayout(effect->GetVertexShader(), &element[0], element.Size());
		vtx.SetInputLayout(layout);
	}

	vtx.SetData((s8*)data.myVertexBuffer);
	vtx.SetStart(vtx_start);
	vtx.SetStride(vtx_Stride);
	vtx.SetByteOffset(0);
	vtx.SetVertexCount(vtx_VertexCount);
	vtx.SetSize(vtx_Size);
	vtx.SetBuffer(buffer);
	vtx.SetTopology(graphics::TRIANGLE_LIST);
#ifdef _DEBUG
	vtx.m_DebugName = DEBUG_NAME_A(data.m_Filename, T);
#endif

}

template<typename T>
void CModelImporter::FillInstanceData(T* out, const ModelData& data, Effect* effect)
{
	auto& ins = out->m_InstanceWrapper;
	const s32 ins_BufferCount = 1;
	const s32 ins_Start = 0;
	const s32 ins_Stride = sizeof(GPUModelData);
	const s32 ins_ByteOffset = 0;
	const s32 ins_InstanceCount = 300;
	const s32 ins_Size = ins_InstanceCount * ins_Stride;
	const s32 ins_IndicesPerInstance = data.myIndexCount;

	graphics::BufferDesc desc;
	desc.m_BindFlag = graphics::BIND_VERTEX_BUFFER;
	desc.m_UsageFlag = graphics::DYNAMIC_USAGE;
	desc.m_CPUAccessFlag = graphics::WRITE;
	desc.m_ByteWidth = ins_Size;

	IBuffer* buffer = Engine::GetAPI()->GetDevice().CreateBuffer(desc, data.m_Filename + "InstanceBuffer");

	CU::GrowingArray<graphics::InputElementDesc> element;
	SetupInputLayout(data, element);
	//Reflect build input layout and compare the self made one from the model?
	// This should definitively create the vertex thing


	/*	
		This is standard for vertex shader and could be a thing to just include as a const and it could be changed based on what content
	*/


	/*
		if I were to write a shader strictly in cpp how would I write it?

		Shader->AddBuffer(cbuffer_struct);
	
		void Shader::AddBuffer(cbuffer_struct) 
		{
			m_BufferCount;
			const char _buffer[] = R"Buffer(

				//Here we would have to break things down


			)Buffer";
			

		}
	
	*/


	const char per_frame[] = R"per_frame(
		
		cbuffer per_frame : register ( b0 );
		{
			row_major float4x4 camera_view_x_proj;
		};		
	

	)per_frame";

	/*const char* per_frame[] = {
		"cbuffer per_frame : register( b0 )\n",
		"{\n",
		"	row_major float4x4 camera_view_x_proj;\n",
		"};\n"
	};


	const char* input_layout[] = {
		"struct vs_input\n",
		"{\n",
	};*/

	//void AddElement(semantic, vertexformat, slot, offset, instanced , semantic_index )

 	graphics::SInputLayout _layout;
	_layout.m_Elements = element;
	_layout.AddElement("INSTANCE",	graphics::_16BYTE_RGBA, 1, true, 0);
	_layout.AddElement("INSTANCE",	graphics::_16BYTE_RGBA, 1, true, 1);
	_layout.AddElement("INSTANCE",	graphics::_16BYTE_RGBA, 1, true, 2);
	_layout.AddElement("INSTANCE",	graphics::_16BYTE_RGBA, 1, true, 3);
	_layout.AddElement("DATA",		graphics::_16BYTE_RGBA, 1, true, 0);
	_layout.AddElement("ID",		graphics::_4BYTE_R_UINT, 1, true, 0);
	_layout.AddElement("HOVER",		graphics::_4BYTE_R_UINT, 1, true, 0);

 	IInputLayout* layout = Engine::GetAPI()->GetDevice().CreateInputLayout(effect->GetVertexShader(), _layout);

	ins.SetBuffer(buffer);
	ins.SetInputLayout(layout);
	ins.SetIndexCountPerInstance(ins_IndicesPerInstance);
	ins.SetInstanceCount(ins_InstanceCount);
	ins.SetByteOffset(ins_ByteOffset);
	ins.SetStride(ins_Stride);
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
	FILE* hFile = fopen(path.c_str(), "rb");
	assert(hFile && "failed to open file!");

	size_t pos = 5;

	if (hFile != nullptr)
	{
		std::ifstream file(path.c_str(), std::ios::binary);

		file.seekg(0, file.end);
		size_t length = file.tellg();
		file.seekg(0, file.beg);

		char* data = new char[length];
		file.read(data, length);
		file.close();


		

		u32 position = 0;
		ReadBlock(data, position, pModel);






		delete data;
		data = nullptr;
	}
}

template <typename T>
void CModelImporter::ReadBlock(const char* data, u32& position, T* pModel)
{

	ModelData model_data; // = new ModelData;

	u32 stride = 0;
	//if (mesh->HasPositions())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_POS;
		newLayout.mySize = VERTEX_STRIDE;
		newLayout.myOffset = 0;
		model_data.myLayout.Add(newLayout);
		stride += VERTEX_STRIDE;
	}

	//if (mesh->HasNormals())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_NORMAL;
		newLayout.mySize = NORMAL_STRIDE;
		newLayout.myOffset = stride * 4;
		model_data.myLayout.Add(newLayout);

		stride += NORMAL_STRIDE;
	}

	//if (mesh->HasTextureCoords(0)) //this is multiple coords 
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_UV;
		newLayout.mySize = UV_STRIDE;
		newLayout.myOffset = stride * 4;
		model_data.myLayout.Add(newLayout);

		stride += UV_STRIDE;
	}

	//if (mesh->HasTangentsAndBitangents())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_BINORMAL;
		newLayout.mySize = BINORMAL_STRIDE;
		newLayout.myOffset = stride * 4;
		model_data.myLayout.Add(newLayout);

		stride += BINORMAL_STRIDE;

		newLayout.myType = ModelData::VERTEX_TANGENT;
		newLayout.mySize = TANGENT_STRIDE;
		newLayout.myOffset = stride * 4;
		model_data.myLayout.Add(newLayout);

		stride += TANGENT_STRIDE;
	}


	model_data.myVertexStride = stride * sizeof(float);

	ReadData(data, position, model_data.myVertexCount);
	model_data.m_VertexBufferSize = model_data.myVertexCount * sizeof(float);
	model_data.myVertexBuffer = new float[model_data.m_VertexBufferSize];

	ZeroMemory(&model_data.myVertexBuffer[0], model_data.m_VertexBufferSize);

	memcpy(&model_data.myVertexBuffer[0], &data[position], model_data.m_VertexBufferSize);

	position += model_data.m_VertexBufferSize;


	ReadData(data, position, model_data.myIndexCount);
	model_data.m_IndexBufferSize = model_data.myIndexCount * sizeof(int);
	model_data.myIndicies = new int[model_data.m_IndexBufferSize];

	ZeroMemory(&model_data.myIndicies[0], model_data.m_IndexBufferSize);
	memcpy(&model_data.myIndicies[0], &data[position], model_data.m_IndexBufferSize);
	position += model_data.m_IndexBufferSize;

	//data.myIndexCount = index_count;
	/*int surface_count = _ReadInt(data, position);
	for (int i = 0; i < surface_count; ++i)
	{
		
		int length = _ReadInt(data, position);
		
		char* resource_name = new char[length];
		memcpy(resource_name, &data[position], sizeof(char) * length);
		position += sizeof(char) * length;

		int slot_type_len = _ReadInt(data, position);
		char* resource_type = new char[slot_type_len];
		memcpy(resource_type, &data[position], sizeof(char) * slot_type_len);
		position += sizeof(char) * slot_type_len;

	}*/
	if(model_data.myVertexCount > 0)
		FillData(model_data, pModel, pModel->m_FileName);
	
	int child_count = 0;
	ReadData(data, position, child_count);
	for (int i = 0; i < child_count; ++i)
	{

		T* child = new T;
		child->m_IsRoot = false;
		pModel->AddChild(child);
		ReadBlock(data, position, child);
	}

}

template <typename T>
void CModelImporter::ReadData(const char* data, u32& position, T& out)
{
	memcpy(&out, &data[position], sizeof(T));
	position += sizeof(T);
}
