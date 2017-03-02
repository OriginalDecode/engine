#include "stdafx.h"
#include "ModelImporter.h"
#include <DL_Debug.h>
#include "Model.h"

#include "VertexWrapper.h"
#include "IndexWrapper.h"
#include "Surface.h"
#include "Model.h"

#include <unordered_map>

#define TRIANGLE_VERTEX_COUNT 3
#define VERTEX_STRIDE 4
#define NORMAL_STRIDE 4
#define BINORMAL_STRIDE 4
#define TANGENT_STRIDE 4
#define SKINWEIGHT_STRIDE 4
#define BONEID_STRIDE 4
#define UV_STRIDE 2


CModelImporter::CModelImporter()
	: myEngine(Engine::GetInstance())
	, myTimeManager(new CommonUtilities::TimeManager())
{
	myTimeManager->CreateTimer();
}


CModelImporter::~CModelImporter()
{
	SAFE_DELETE(myTimeManager);
}

CModel* CModelImporter::CreateModel(FBXModelData* someData, Effect* anEffect)
{

	CModel* newModel = new CModel();
	newModel->SetEffect(anEffect);

	if (someData->myData)
	{
		FillData(someData, newModel, anEffect);
		newModel->myOrientation = someData->myOrientation;
	}
	for (FBXModelData* child : someData->myChildren)
	{
		newModel->AddChild(CreateModel(child, anEffect));
	}

	newModel->SetWHD(m_WHD);

	return newModel;
}

CModel* CModelImporter::LoadModel(const std::string& aFilePath, const std::string& aEffectPath)
{
	m_WHD = { 0.f,0.f,0.f };
	myCurrentLoadingFile = aFilePath;
	CModel* model = LoadModel(aFilePath, myEngine->GetEffect(aEffectPath))->CreateModel(aFilePath);
	return model;
}

CModel* CModelImporter::LoadModel(const std::string& aFilePath, Effect* anEffect)
{
	myTimeManager->Update();
	float loadTime = myTimeManager->GetTimer(0).GetTotalTime().GetMilliseconds();

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

	const aiScene* scene = importer.ReadFile(aFilePath, processFlags);

	DL_MESSAGE_EXP(!scene , "%s", importer.GetErrorString());
	DL_ASSERT_EXP(scene, "ImportModel Failed. Could not read the requested file.");

	aiNode* rootNode = scene->mRootNode;
	FBXModelData* data = new FBXModelData;

	ProcessNode(rootNode, scene, data);

	CModel* toReturn = CreateModel(data, anEffect);


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

	myTimeManager->Update();
	loadTime = myTimeManager->GetTimer(0).GetTotalTime().GetMilliseconds() - loadTime;
	MODEL_LOG("%s took %fms to load. %s", aFilePath.c_str(), loadTime, (loadTime > 7000.f) ? "Check if it's saved as binary." : 0);

	return toReturn;
}

void CModelImporter::FillData(FBXModelData* someData, CModel* out, Effect* /*anEffect*/)
{
#ifdef SNOWBLIND_DX11
	ModelData* data = someData->myData;

	if (m_WHD.x <= 0.f + FLT_EPSILON)
		m_WHD.x += 0.1f;
	if (m_WHD.y <= 0.f + FLT_EPSILON)
		m_WHD.y += 0.1f;
	if (m_WHD.z <= 0.f + FLT_EPSILON)
		m_WHD.z += 0.1f;


	
	out->SetWHD(m_WHD);
	//VertexIndexWrapper* indexWrapper = new VertexIndexWrapper();
	out->m_IndexData.myFormat = DXGI_FORMAT_R32_UINT;
	u32* indexData = new u32[data->myIndexCount];
	memcpy(indexData, data->myIndicies, data->myIndexCount * sizeof(u32));
	out->m_IndexData.myIndexData = (s8*)indexData;
	out->m_IndexData.mySize = data->myIndexCount * sizeof(u32);
	//out->m_IndexData = indexWrapper;

	for (u32 i = 0; i < data->myIndexCount; i++)
	{
		out->m_Indices.Add(data->myIndicies[i]);
	}



	/* BUG HERE. CRASH. */
	//VertexDataWrapper* vertexData = new VertexDataWrapper();
	s32 sizeOfBuffer = data->myVertexCount * data->myVertexStride * sizeof(float); //is this wrong?
	u32* vertexRawData = new u32[sizeOfBuffer];
	memcpy(vertexRawData, data->myVertexBuffer, sizeOfBuffer); // This crashes?
	out->m_VertexData.myVertexData = (s8*)vertexRawData;
	out->m_VertexData.myNrOfVertexes = data->myVertexCount;
	out->m_VertexData.mySize = sizeOfBuffer;
	out->m_VertexData.myStride = data->myVertexStride * sizeof(float);
	//out->myVertexData = vertexData;

	CSurface* newSurface = new CSurface(0, data->myVertexCount, 0
		, data->myIndexCount, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	out->myIsNULLObject = false;
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


	const CU::GrowingArray<TextureInfo>& info = someData->myTextureData->myTextures;

	for (s32 i = 0; i < info.Size(); i++)
	{
		newSurface->AddTexture(info[i].myFilename, (TextureType)info[i].myType);
	}


	if(!CL::substr(myCurrentLoadingFile, "Skysphere"))
		newSurface->ValidateTextures();

	//newSurface->ValidateTextures();

	out->mySurfaces.Add(newSurface);
#endif
}

void CModelImporter::ProcessNode(aiNode* aNode, const aiScene* aScene, FBXModelData* someData)
{
#ifdef SNOWBLIND_DX11
	DL_ASSERT_EXP(someData, "Failed to process node. FBXModelData someData was null");

	for (u32 i = 0; i < aNode->mNumMeshes; i++)
	{
		aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
		ProcessMesh(mesh, aScene, someData);
	}

	for (u32 i = 0; i < aNode->mNumChildren; i++)
	{
		someData->myChildren.Add(new FBXModelData);
		ProcessNode(aNode->mChildren[i], aScene, someData->myChildren.GetLast());
	}
#endif
}

void CModelImporter::ProcessMesh(aiMesh* aMesh, const aiScene* aScene, FBXModelData* fbx)
{
#ifdef SNOWBLIND_DX11
	FBXModelData* data = fbx;
	data->myData = new ModelData();


	u32 polygonCount = aMesh->mNumFaces;
	u32 size = polygonCount * VERTEX_STRIDE;
	u32 polygonVertexCount = polygonCount * 4;

	if (aMesh->HasPositions())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_POS;
		newLayout.mySize = VERTEX_STRIDE;
		newLayout.myOffset = 0;
		data->myData->myLayout.Add(newLayout);
		size += polygonVertexCount * VERTEX_STRIDE;
	}
	u32 stride = VERTEX_STRIDE;


	if (aMesh->HasNormals())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_NORMAL;
		newLayout.mySize = NORMAL_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += NORMAL_STRIDE;
		size += polygonVertexCount * NORMAL_STRIDE;
	}

	if (aMesh->HasTextureCoords(0))
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_UV;
		newLayout.mySize = UV_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += UV_STRIDE;
		size += polygonVertexCount * UV_STRIDE;
	}

	if (aMesh->HasTangentsAndBitangents())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_BINORMAL;
		newLayout.mySize = BINORMAL_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += BINORMAL_STRIDE;
		size += polygonVertexCount * BINORMAL_STRIDE;

		newLayout.myType = ModelData::VERTEX_TANGENT;
		newLayout.mySize = TANGENT_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += TANGENT_STRIDE;
		size += polygonVertexCount * TANGENT_STRIDE;
	}

	if (aMesh->HasBones())
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_SKINWEIGHTS;
		newLayout.mySize = SKINWEIGHT_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += SKINWEIGHT_STRIDE;
		size += polygonVertexCount * SKINWEIGHT_STRIDE;

		newLayout.myType = ModelData::VERTEX_BONEID;
		newLayout.mySize = BONEID_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += BONEID_STRIDE;
		size += polygonVertexCount * BONEID_STRIDE;
	}

	DL_MESSAGE("Vertex Buffer Array Size : %d", size);
	data->myData->myVertexBuffer = new float[size];
	ZeroMemory(data->myData->myVertexBuffer, sizeof(float)*size);

	DL_ASSERT_EXP(aMesh->mNumVertices < size, "the amount of vertices was MORE!? than size");
	data->myData->myIndicies = new u32[polygonCount * 3];
	ZeroMemory(data->myData->myIndicies, sizeof(u32) * polygonCount * 3);

	CU::GrowingArray<u32> indices;
	u32 vertCount = 0;

	float w = 0.f;
	float h = 0.f;
	float d = 0.f;

	for (u32 i = 0; i < aMesh->mNumFaces; i++)
	{
		const aiFace* face = &aMesh->mFaces[i];

		for (s32 j = 2; j >= 0; j--)
		{
			u32 addedSize = VERTEX_STRIDE;
			u32 currIndex = vertCount * stride;
			DL_ASSERT_EXP(addedSize <= size, "addedSize was larger than the size of the array.");
			u32 verticeIndex = face->mIndices[j];

			Vertex vert;

			if (aMesh->HasPositions())
			{
				CU::Vector4f position(aMesh->mVertices[verticeIndex].x, aMesh->mVertices[verticeIndex].y, aMesh->mVertices[verticeIndex].z, 1);
				CU::Matrix44f fixMatrix = CU::Math::CreateReflectionMatrixAboutAxis44(CU::Vector3f(1, 0, 0));
				position = position * fixMatrix;

				data->myData->myVertexBuffer[currIndex] = position.x;
				data->myData->myVertexBuffer[currIndex + 1] = position.y;
				data->myData->myVertexBuffer[currIndex + 2] = position.z;
				data->myData->myVertexBuffer[currIndex + 3] = 0;

				if (i != 0)
				{
					float temp_x, temp_y, temp_z;
					if (position.x < 0.f)
					{
						temp_x = -position.x;

						if (w < temp_x)
							w = temp_x;
					}

					if (position.y < 0.f)
					{
						temp_y = -position.y;

						if (w < temp_y)
							w = temp_y;
					}

					if (position.z < 0.f)
					{
						temp_z = -position.z;

						if (d < temp_z)
							d = temp_z;
					}

					if (w < position.x)
						w = position.x;
					if (h < position.y)
						h = position.y;
					if (d < position.z)
						d = position.z;
				}
				else
				{
					
					w = position.x;
					h = position.y;
					d = position.z;
				}
			}

			if (aMesh->HasNormals())
			{

				CU::Vector3f normal(
					aMesh->mNormals[verticeIndex].x,
					aMesh->mNormals[verticeIndex].y,
					aMesh->mNormals[verticeIndex].z);
				normal = normal * CU::Math::CreateReflectionMatrixAboutAxis(CU::Vector3f(1, 0, 0));
				CU::Math::Normalize(normal);


				data->myData->myVertexBuffer[currIndex + addedSize] = normal.x;
				data->myData->myVertexBuffer[currIndex + addedSize + 1] = normal.y;
				data->myData->myVertexBuffer[currIndex + addedSize + 2] = normal.z;
				data->myData->myVertexBuffer[currIndex + addedSize + 3] = 0;
				addedSize += NORMAL_STRIDE;
			}

			if (aMesh->HasTextureCoords(0))
			{
				CU::Vector2f uv(aMesh->mTextureCoords[0][verticeIndex].x, aMesh->mTextureCoords[0][verticeIndex].y * -1.f);


				data->myData->myVertexBuffer[currIndex + addedSize] = uv.x;
				data->myData->myVertexBuffer[currIndex + addedSize + 1] = uv.y;
				addedSize += UV_STRIDE;
			}

			if (aMesh->HasTangentsAndBitangents())
			{

				CU::Vector3f binorm(
					aMesh->mBitangents[verticeIndex].x,
					aMesh->mBitangents[verticeIndex].y,
					aMesh->mBitangents[verticeIndex].z);
				binorm = binorm * CU::Math::CreateReflectionMatrixAboutAxis(CU::Vector3f(1, 0, 0));
				CU::Math::Normalize(binorm);

				data->myData->myVertexBuffer[currIndex + addedSize] = binorm.x;
				data->myData->myVertexBuffer[currIndex + addedSize + 1] = binorm.y;
				data->myData->myVertexBuffer[currIndex + addedSize + 2] = binorm.z;
				data->myData->myVertexBuffer[currIndex + addedSize + 3] = 0;
				addedSize += BINORMAL_STRIDE;

				CU::Vector3f tangent(
					aMesh->mTangents[verticeIndex].x, 
					aMesh->mTangents[verticeIndex].y,
					aMesh->mTangents[verticeIndex].z);
				tangent = tangent * CU::Math::CreateReflectionMatrixAboutAxis(CU::Vector3f(-1, 0, 0));
				CU::Math::Normalize(tangent);


				data->myData->myVertexBuffer[currIndex + addedSize] = tangent.x;
				data->myData->myVertexBuffer[currIndex + addedSize + 1] = tangent.y;
				data->myData->myVertexBuffer[currIndex + addedSize + 2] = tangent.z;
				data->myData->myVertexBuffer[currIndex + addedSize + 3] = 0;
				addedSize += TANGENT_STRIDE;
			}

			indices.Add(verticeIndex);
			vertCount++;
		}
	}

	if (w > m_WHD.x)
	{
		m_WHD.x = w;
	}

	if (h > m_WHD.y)
	{
		m_WHD.y = h;
	}

	if (d > m_WHD.z)
	{
		m_WHD.z = d;
	}


	//data->myData->m_WHD.x = w;
	//data->myData->m_WHD.y = h;
	//data->myData->m_WHD.z = d;

	//Flips it to make it correct.
	CU::GrowingArray<u32> indiceFix;
	for (s32 i = indices.Size() - 1; i >= 0; i--)
	{
		indiceFix.Add(indices[i]);
	}

	memcpy(data->myData->myIndicies, &indiceFix[0], sizeof(u32) * indiceFix.Size());
	data->myData->myVertexStride = stride;

	data->myData->myVertexCount = vertCount;
	data->myData->myIndexCount = indiceFix.Size();


	data->myTextureData = new TextureData;

	aiMaterial* material = aScene->mMaterials[aMesh->mMaterialIndex];
	for (u32 i = 0; i < material->mNumProperties; i++)
	{
		aiMaterialProperty* prop = material->mProperties[i];
		u32 type = prop->mSemantic;
		aiString str;
		material->GetTexture(static_cast<aiTextureType>(type), 0, &str);

		std::string newPath = CL::substr(myCurrentLoadingFile, "/", true, 0);

		std::string fileName = CL::substr(str.C_Str(), "\\", false, 0);
		if (CL::substr(str.C_Str(), "\\"))
		{
			fileName.erase(0, 1);
		}
		newPath += "/";
		newPath += fileName;
		if (fileName != "")
		{
			//myEngine->GetTexture(newPath); //All textures now get properly loaded.
			TextureInfo newInfo;
			newInfo.myFilename = newPath;


			aiTextureType lType = static_cast<aiTextureType>(type);
			//DL_MESSAGE("Type : %d, Name : %s", u32(lType), newPath.c_str());
			switch (lType)
			{
				case aiTextureType_DIFFUSE:
				{
					newInfo.myType = TextureType::_ALBEDO;
				}break;

				case aiTextureType_SPECULAR:
				{
					newInfo.myType = TextureType::_ROUGHNESS;
				}break;

				case aiTextureType_AMBIENT:
				{
					newInfo.myType = TextureType::_AO;
				}break;

				case aiTextureType_EMISSIVE:
				{
					newInfo.myType = TextureType::_EMISSIVE;
				}break;

				case aiTextureType_HEIGHT:
				{
					newInfo.myType = TextureType::_HEIGHT;
				}break;

				case aiTextureType_NORMALS:
				{
					newInfo.myType = TextureType::_NORMAL;
				}break;

				case aiTextureType_SHININESS:
				{
					newInfo.myType = TextureType::_SHININESS;
				}break;

				case aiTextureType_OPACITY:
				{
					newInfo.myType = TextureType::_OPACITY;
				}break;

				case aiTextureType_DISPLACEMENT:
				{
					newInfo.myType = TextureType::_DISPLACEMENT;
				}break;

				case aiTextureType_LIGHTMAP:
				{
					newInfo.myType = TextureType::_LIGHTMAP;
				}break;

				case aiTextureType_REFLECTION:
				{
					newInfo.myType = TextureType::_METALNESS;
				}break;

				case aiTextureType_UNKNOWN:
				{
					DL_ASSERT("Not implemented");
				}break;
			}

			bool found = false;


			const CU::GrowingArray<TextureInfo> texInfo = data->myTextureData->myTextures;
			for (const TextureInfo& info : texInfo)
			{
				if (info.myFilename == newInfo.myFilename && info.myType == newInfo.myType)
				{
					found = true;
					break;
				}
			}
			
			if (found == false)
			{
				data->myTextureData->myTextures.Add(newInfo);
			}

		}
	}
#endif
}

bool operator==(const Vertex& aVertice, const Vertex& aSecond)
{

	if (aVertice.position != aSecond.position)
		return false;

	if (aVertice.normal != aSecond.normal)
		return false;

	if (aVertice.uv != aSecond.uv)
		return false;

	if (aVertice.binormal != aSecond.binormal)
		return false;

	if (aVertice.tangent != aSecond.tangent)
		return false;


	return true;
}

bool operator!=(const Vertex& aVertice, const Vertex& aSecond)
{
	return !(aVertice == aSecond);
}
