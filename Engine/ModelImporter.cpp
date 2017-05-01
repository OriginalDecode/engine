#include "stdafx.h"
#include "ModelImporter.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Engine/Model.h>

#define TRIANGLE_VERTEX_COUNT 3
#define VERTEX_STRIDE 4
#define NORMAL_STRIDE 4
#define BINORMAL_STRIDE 4
#define TANGENT_STRIDE 4
#define SKINWEIGHT_STRIDE 4
#define BONEID_STRIDE 4
#define UV_STRIDE 2


CModelImporter::CModelImporter()
	: m_Engine(Engine::GetInstance())
{
#ifdef _DEBUG
	m_TimeManager.CreateTimer();
#endif
}

void CModelImporter::LoadModel(std::string aFilePath, Model* model, std::string effect)
{
	BeginTicketMutex(&m_LoaderMutex);

	LoadModel(aFilePath, model, m_Engine->GetEffect(effect));
	model->Initiate(aFilePath);

	EndTicketMutex(&m_LoaderMutex);
}

Model* CModelImporter::LoadModel(std::string filepath, Model* model, Effect* anEffect)
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

	Model* toReturn = CreateModel(data, model, filepath, anEffect);


	if ( data->myTextureData )
	{
		delete data->myTextureData;
	}
	if ( data->myData )
	{
		delete[] data->myData->myIndicies;
		delete[] data->myData->myVertexBuffer;
	}
	if ( data )
	{
		delete data->myData;
	}
	delete data;

#ifdef _DEBUG
	m_TimeManager.Update();
	loadTime = m_TimeManager.GetTimer(0).GetTotalTime().GetMilliseconds() - loadTime;
	MODEL_LOG("%s took %fms to load. %s", filepath.c_str(), loadTime, ( loadTime > 7000.f ) ? "Check if it's saved as binary." : 0);
#endif
	return toReturn;
}


Model* CModelImporter::CreateModel(FBXModelData* someData, Model* model, std::string filepath, Effect* effect)
{
	model->SetEffect(effect);

	if ( someData->myData )
	{
		FillData(someData, model, filepath);
		model->myOrientation = someData->myOrientation;
	}

	for ( FBXModelData* child : someData->myChildren )
	{
		model->AddChild(CreateChild(child, filepath, effect));
	}

	return 0;
}

Model* CModelImporter::CreateChild(FBXModelData* data, std::string filepath, Effect* effect)
{
	Model* model = new Model;

	size_t pos = filepath.rfind('/');
	
	model->m_Filename = filepath.substr(pos);
	model->SetEffect(effect);

	if ( data->myData )
	{
		FillData(data, model, filepath);
		model->myOrientation = data->myOrientation;
	}

	for ( FBXModelData* child : data->myChildren )
	{
		model->AddChild(CreateChild(child, filepath, effect));
	}

	return model;
}

void CModelImporter::FillData(FBXModelData* someData, Model* out, std::string filepath)
{
#ifdef SNOWBLIND_DX11
	ModelData* data = someData->myData;

	out->m_IndexData.myFormat = DXGI_FORMAT_R32_UINT;
	u32* indexData = new u32[data->myIndexCount];
	memcpy(indexData, data->myIndicies, data->myIndexCount * sizeof(u32));
	out->m_IndexData.myIndexData = ( s8* ) indexData;
	out->m_IndexData.mySize = data->myIndexCount * sizeof(u32);

	for ( u32 i = 0; i < data->myIndexCount; i++ )
	{
		out->m_Indices.Add(data->myIndicies[i]);
	}



	s32 sizeOfBuffer = data->myVertexCount * data->myVertexStride * sizeof(float); //is this wrong?
	u32* vertexRawData = new u32[sizeOfBuffer];
	memcpy(vertexRawData, data->myVertexBuffer, sizeOfBuffer); // This crashes?
	out->m_VertexData.myVertexData = ( s8* ) vertexRawData;
	out->m_VertexData.myNrOfVertexes = data->myVertexCount;
	out->m_VertexData.mySize = sizeOfBuffer;
	out->m_VertexData.myStride = data->myVertexStride * sizeof(float);

	CSurface* newSurface = new CSurface(0, data->myVertexCount, 0
		, data->myIndexCount, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	out->m_IsRoot = false;
	SetupInputLayout(data, out);


	const CU::GrowingArray<TextureInfo>& info = someData->myTextureData->myTextures;

	for ( s32 i = 0; i < info.Size(); i++ )
	{
		newSurface->AddTexture(info[i].myFilename, ( TextureType ) info[i].myType);
	}

	if( filepath.find("Skysphere") == filepath.npos	)
		newSurface->ValidateTextures();

	//newSurface->ValidateTextures();
	newSurface->Optimize();
	out->mySurfaces.Add(newSurface);
#endif
}

void CModelImporter::SetupInputLayout(ModelData* data, Model* out)
{
	for ( int i = 0; i < data->myLayout.Size(); ++i )
	{
		auto currentLayout = data->myLayout[i];
		D3D11_INPUT_ELEMENT_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_INPUT_ELEMENT_DESC));
		desc.SemanticIndex = 0;
		desc.AlignedByteOffset = currentLayout.myOffset;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InputSlot = 0;
		desc.InstanceDataStepRate = 0;

		if ( currentLayout.myType == ModelData::VERTEX_POS )
		{
			desc.SemanticName = "POSITION";
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if ( currentLayout.myType == ModelData::VERTEX_NORMAL )
		{
			desc.SemanticName = "NORMAL";
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if ( currentLayout.myType == ModelData::VERTEX_UV )
		{
			desc.SemanticName = "TEXCOORD";
			desc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if ( currentLayout.myType == ModelData::VERTEX_BINORMAL )
		{
			desc.SemanticName = "BINORMAL";
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if ( currentLayout.myType == ModelData::VERTEX_TANGENT )
		{
			desc.SemanticName = "TANGENT";
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if ( currentLayout.myType == ModelData::VERTEX_SKINWEIGHTS )
		{
			break;
			desc.SemanticName = "WEIGHTS";
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		else if ( currentLayout.myType == ModelData::VERTEX_BONEID )
		{
			break;
			desc.SemanticName = "BONES";
			desc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
		}
		out->myVertexFormat.Add(desc);

	}
}

void CModelImporter::ProcessNode(aiNode* aNode, const aiScene* aScene, FBXModelData* someData, std::string file)
{
#ifdef SNOWBLIND_DX11
	DL_ASSERT_EXP(someData, "Failed to process node. FBXModelData someData was null");

	for ( u32 i = 0; i < aNode->mNumMeshes; i++ )
	{
		aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
		ProcessMesh(mesh, aScene, someData, file);
	}

	for ( u32 i = 0; i < aNode->mNumChildren; i++ )
	{
		someData->myChildren.Add(new FBXModelData);
		ProcessNode(aNode->mChildren[i], aScene, someData->myChildren.GetLast(), file);
	}
#endif
}

void CModelImporter::ProcessMesh(aiMesh* mesh, const aiScene* scene, FBXModelData* fbx, std::string file)
{
	FBXModelData* data = fbx;
	data->myData = new ModelData;

	u32 polygonCount = mesh->mNumFaces;
	u32 size = polygonCount * VERTEX_STRIDE;
	u32 polygonVertexCount = polygonCount * 4;

	u32 stride = 0;
	if ( mesh->HasPositions() )
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_POS;
		newLayout.mySize = VERTEX_STRIDE;
		newLayout.myOffset = 0;
		data->myData->myLayout.Add(newLayout);
		size += polygonVertexCount * VERTEX_STRIDE;
		stride += VERTEX_STRIDE;
	}

	if ( mesh->HasNormals() )
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_NORMAL;
		newLayout.mySize = NORMAL_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += NORMAL_STRIDE;
		size += polygonVertexCount * NORMAL_STRIDE;
	}

	if ( mesh->HasTextureCoords(0) )
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_UV;
		newLayout.mySize = UV_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myData->myLayout.Add(newLayout);

		stride += UV_STRIDE;
		size += polygonVertexCount * UV_STRIDE;
	}

	if ( mesh->HasTangentsAndBitangents() )
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

	if ( mesh->HasBones() )
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

	DL_ASSERT_EXP(mesh->mNumVertices < size, "the amount of vertices was MORE!? than size");
	data->myData->myIndicies = new u32[polygonCount * 3];
	ZeroMemory(data->myData->myIndicies, sizeof(u32) * polygonCount * 3);

	CU::GrowingArray<u32> indices;
	u32 vertCount = 0;

	CU::Vector3f min_point, max_point;

	for ( u32 i = 0; i < mesh->mNumFaces; i++ )
	{
		const aiFace* face = &mesh->mFaces[i];

		for ( s32 j = 2; j >= 0; j-- )
		{
			u32 addedSize = VERTEX_STRIDE;
			u32 currIndex = vertCount * stride;
			DL_ASSERT_EXP(addedSize <= size, "addedSize was larger than the size of the array.");
			u32 verticeIndex = face->mIndices[j];

			if ( mesh->HasPositions() )
			{
				CU::Vector4f position(mesh->mVertices[verticeIndex].x, mesh->mVertices[verticeIndex].y, mesh->mVertices[verticeIndex].z, 1);
				CU::Matrix44f fixMatrix = CU::Math::CreateReflectionMatrixAboutAxis44(CU::Vector3f(1, 0, 0));
				position = position * fixMatrix;

				data->myData->myVertexBuffer[currIndex] = position.x;
				data->myData->myVertexBuffer[currIndex + 1] = position.y;
				data->myData->myVertexBuffer[currIndex + 2] = position.z;
				data->myData->myVertexBuffer[currIndex + 3] = 0;

				if ( i != 0 )
				{
					if ( position.x <= min_point.x )
						min_point.x = position.x;

					if ( position.x > max_point.x )
						max_point.x = position.x;

					if ( position.y <= min_point.y )
						min_point.y = position.y;

					if ( position.y > max_point.y )
						max_point.y = position.y;

					if ( position.z <= min_point.z )
						min_point.z = position.z;

					if ( position.z > max_point.z )
						max_point.z = position.z;
				}

				if ( mesh->HasNormals() )
				{

					CU::Vector3f normal(
						mesh->mNormals[verticeIndex].x,
						mesh->mNormals[verticeIndex].y,
						mesh->mNormals[verticeIndex].z);
					normal = normal * CU::Math::CreateReflectionMatrixAboutAxis(CU::Vector3f(1, 0, 0));
					CU::Math::Normalize(normal);


					data->myData->myVertexBuffer[currIndex + addedSize] = normal.x;
					data->myData->myVertexBuffer[currIndex + addedSize + 1] = normal.y;
					data->myData->myVertexBuffer[currIndex + addedSize + 2] = normal.z;
					data->myData->myVertexBuffer[currIndex + addedSize + 3] = 0;
					addedSize += NORMAL_STRIDE;
				}

				if ( mesh->HasTextureCoords(0) )
				{
					CU::Vector2f uv(mesh->mTextureCoords[0][verticeIndex].x, mesh->mTextureCoords[0][verticeIndex].y * -1.f);


					data->myData->myVertexBuffer[currIndex + addedSize] = uv.x;
					data->myData->myVertexBuffer[currIndex + addedSize + 1] = uv.y;
					addedSize += UV_STRIDE;
				}

				if ( mesh->HasTangentsAndBitangents() )
				{

					CU::Vector3f binorm(
						mesh->mBitangents[verticeIndex].x,
						mesh->mBitangents[verticeIndex].y,
						mesh->mBitangents[verticeIndex].z);
					binorm = binorm * CU::Math::CreateReflectionMatrixAboutAxis(CU::Vector3f(1, 0, 0));
					CU::Math::Normalize(binorm);

					data->myData->myVertexBuffer[currIndex + addedSize] = binorm.x;
					data->myData->myVertexBuffer[currIndex + addedSize + 1] = binorm.y;
					data->myData->myVertexBuffer[currIndex + addedSize + 2] = binorm.z;
					data->myData->myVertexBuffer[currIndex + addedSize + 3] = 0;
					addedSize += BINORMAL_STRIDE;

					CU::Vector3f tangent(
						mesh->mTangents[verticeIndex].x,
						mesh->mTangents[verticeIndex].y,
						mesh->mTangents[verticeIndex].z);
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

		data->myData->m_MinPoint = min_point;
		data->myData->m_MaxPoint = max_point;


		//Flips it to make it correct.
		CU::GrowingArray<u32> indiceFix;
		for ( s32 indice = indices.Size() - 1; indice >= 0; indice-- )
		{
			indiceFix.Add(indices[indice]);
		}

		memcpy(data->myData->myIndicies, &indiceFix[0], sizeof(u32) * indiceFix.Size());

		data->myData->myVertexStride = stride;
		data->myData->myVertexCount = vertCount;
		data->myData->myIndexCount =  indiceFix.Size();
		
	}
	
	ExtractMaterials(mesh, scene, data, file);

}

void CModelImporter::ExtractMaterials(aiMesh* mesh, const aiScene* scene, FBXModelData* data, std::string file)
{
	data->myTextureData = new TextureData;

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	for ( u32 i = 0; i < material->mNumProperties; i++ )
	{
		aiMaterialProperty* prop = material->mProperties[i];
		u32 type = prop->mSemantic;
		aiString str;
		material->GetTexture(static_cast< aiTextureType >( type ), 0, &str);
		std::string newPath = CL::substr(file, "/", true, 0);

		std::string fileName = CL::substr(str.C_Str(), "\\", false, 0);
		if ( fileName.empty() )
			fileName = CL::substr(str.C_Str(), "/", false, 0);
		if ( CL::substr(str.C_Str(), "\\") )
		{
			fileName.erase(0, 1);
		}
		newPath += "/";
		newPath += fileName;
		if ( fileName != "" )
		{
			//myEngine->GetTexture(newPath); //All textures now get properly loaded.
			TextureInfo newInfo;
			newInfo.myFilename = newPath;


			aiTextureType lType = static_cast< aiTextureType >( type );
			//DL_MESSAGE("Type : %d, Name : %s", u32(lType), newPath.c_str());
			switch ( lType )
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
			for ( const TextureInfo& info : texInfo )
			{
				if ( info.myFilename == newInfo.myFilename && info.myType == newInfo.myType )
				{
					found = true;
					break;
				}
			}

			if ( found == false )
			{
				data->myTextureData->myTextures.Add(newInfo);
			}

		}
	}
}
