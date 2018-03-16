#include "stdafx.h"
#include "ModelImporter.h"


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


//#define THREAD_IF(func, statement) if(statement) m_Engine->GetThreadpool().AddWork(Work([&]() { func; })); else func;

bool CModelImporter::ProcessNode(aiNode* aNode, const aiScene* aScene, FBXModelData* someData, std::string file)
{
	DL_ASSERT_EXP(someData, "Failed to process node. FBXModelData someData was null");
	DL_ASSERT_EXP(aScene, "Scene is null");

	while (m_Pool.HasWork())
	{
		m_Pool.Update();
	}

	for (u32 i = 0; i < aNode->mNumMeshes; i++)
	{
		aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
		ProcessMesh(mesh, aScene, someData, file);
		//DL_ASSERT_EXP(someData->myData, "Was null after ProcessMesh!?");
	}


	bool thread = true;
	bool thread2 = false;
	s32 _meshes_done = 0;
	for (u32 i = 0; i < aNode->mNumMeshes; i++)
	{
		if (thread)
		{
			m_Pool.AddWork(Work([&, i] 
			{
				unsigned int index = aNode->mMeshes[i];
				aiMesh* mesh = aScene->mMeshes[index];
				ProcessMesh(mesh, aScene, someData, file);
				//DL_ASSERT_EXP(someData->myData, "Was null after ProcessMesh!?");
				_meshes_done++;
			}));
		}
		else
		{
			aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
			ProcessMesh(mesh, aScene, someData, file);
			//DL_ASSERT_EXP(someData->myData, "Was null after ProcessMesh!?");
			_meshes_done++;
		}
	}

	for ( u32 i = 0; i < aNode->mNumChildren; i++ )
	{
		someData->myChildren.Add(new FBXModelData);
		FBXModelData* data = someData->myChildren.GetLast();
		aiNode* node = aNode->mChildren[i];
		if (thread2)
		{
			m_Pool.AddWork(Work([=]() {
				ProcessNode(node, aScene, data, file);
			}));
		}
		else
		{
			ProcessNode(node, aScene, someData->myChildren.GetLast(), file);
		}
	}

	return true;

}


void CModelImporter::ProcessMesh(aiMesh* mesh, const aiScene* scene, FBXModelData* fbx, std::string file)
{
	//FBXModelData* data = fbx;
	ModelData* data = new ModelData;

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
		data->myLayout.Add(newLayout);
		size += polygonVertexCount * VERTEX_STRIDE;
		stride += VERTEX_STRIDE;
	}

	if ( mesh->HasNormals() )
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_NORMAL;
		newLayout.mySize = NORMAL_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myLayout.Add(newLayout);

		stride += NORMAL_STRIDE;
		size += polygonVertexCount * NORMAL_STRIDE;
	}

	if ( mesh->HasTextureCoords(0) ) //this is multiple coords 
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_UV;
		newLayout.mySize = UV_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myLayout.Add(newLayout);

		stride += UV_STRIDE;
		size += polygonVertexCount * UV_STRIDE;
	}

	if ( mesh->HasTangentsAndBitangents() )
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_BINORMAL;
		newLayout.mySize = BINORMAL_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myLayout.Add(newLayout);

		stride += BINORMAL_STRIDE;
		size += polygonVertexCount * BINORMAL_STRIDE;

		newLayout.myType = ModelData::VERTEX_TANGENT;
		newLayout.mySize = TANGENT_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myLayout.Add(newLayout);

		stride += TANGENT_STRIDE;
		size += polygonVertexCount * TANGENT_STRIDE;
	}

	if ( mesh->HasBones() )
	{
		ModelData::Layout newLayout;
		newLayout.myType = ModelData::VERTEX_SKINWEIGHTS;
		newLayout.mySize = SKINWEIGHT_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myLayout.Add(newLayout);

		stride += SKINWEIGHT_STRIDE;
		size += polygonVertexCount * SKINWEIGHT_STRIDE;

		newLayout.myType = ModelData::VERTEX_BONEID;
		newLayout.mySize = BONEID_STRIDE;
		newLayout.myOffset = stride * 4;
		data->myLayout.Add(newLayout);

		stride += BONEID_STRIDE;
		size += polygonVertexCount * BONEID_STRIDE;
	}

	DL_MESSAGE("Vertex Buffer Array Size : %d", size);

	const u32 vtx_size = size;
	data->myVertexBuffer = new float[vtx_size];
	ZeroMemory(data->myVertexBuffer, sizeof(float) * vtx_size);
	data->m_VertexBufferSize = sizeof(float)*vtx_size;
	DL_ASSERT_EXP(mesh->mNumVertices < size, "the amount of vertices was MORE!? than size");

	CU::GrowingArray<u32> indices(polygonCount * 3);
	u32 vertCount = 0;

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
				CU::Vector4f position(
					mesh->mVertices[verticeIndex].x, 
					mesh->mVertices[verticeIndex].y,
					mesh->mVertices[verticeIndex].z, 
					1);
				/*CU::Matrix44f fixMatrix = CU::Math::CreateReflectionMatrixAboutAxis44(CU::Vector3f(1, 0, 0));
				position = position * fixMatrix;*/

				data->myVertexBuffer[currIndex] = position.x;
				data->myVertexBuffer[currIndex + 1] = position.y;
				data->myVertexBuffer[currIndex + 2] = position.z;
				data->myVertexBuffer[currIndex + 3] = 1;


			/*	if (i != 0)
				{

					min_point.x = min(position.x, min_point.x);
					min_point.y = min(position.y, min_point.y);
					min_point.z = min(position.z, min_point.z);

					max_point.x = max(position.x, max_point.x);
					max_point.y = max(position.y, max_point.y);
					max_point.z = max(position.z, max_point.z);
				}*/
					

				if ( mesh->HasNormals() )
				{

					CU::Vector3f normal(
						mesh->mNormals[verticeIndex].x,
						mesh->mNormals[verticeIndex].y,
						mesh->mNormals[verticeIndex].z);
					/*normal = normal * CU::Math::CreateReflectionMatrixAboutAxis(CU::Vector3f(1, 0, 0));
					CU::Math::Normalize(normal);*/


					data->myVertexBuffer[currIndex + addedSize] = normal.x;
					data->myVertexBuffer[currIndex + addedSize + 1] = normal.y;
					data->myVertexBuffer[currIndex + addedSize + 2] = normal.z;
					data->myVertexBuffer[currIndex + addedSize + 3] = 0;
					addedSize += NORMAL_STRIDE;
				}

				if ( mesh->HasTextureCoords(0) )
				{
					CU::Vector2f uv(mesh->mTextureCoords[0][verticeIndex].x, mesh->mTextureCoords[0][verticeIndex].y * -1.f);


					data->myVertexBuffer[currIndex + addedSize] = uv.x;
					data->myVertexBuffer[currIndex + addedSize + 1] = uv.y;
					addedSize += UV_STRIDE;
				}

				if ( mesh->HasTangentsAndBitangents() )
				{

					CU::Vector3f binorm(
						mesh->mBitangents[verticeIndex].x,
						mesh->mBitangents[verticeIndex].y,
						mesh->mBitangents[verticeIndex].z);
					/*binorm = binorm * CU::Math::CreateReflectionMatrixAboutAxis(CU::Vector3f(1, 0, 0));
					CU::Math::Normalize(binorm);*/

					data->myVertexBuffer[currIndex + addedSize] = binorm.x;
					data->myVertexBuffer[currIndex + addedSize + 1] = binorm.y;
					data->myVertexBuffer[currIndex + addedSize + 2] = binorm.z;
					data->myVertexBuffer[currIndex + addedSize + 3] = 0;
					addedSize += BINORMAL_STRIDE;

					CU::Vector3f tangent(
						mesh->mTangents[verticeIndex].x,
						mesh->mTangents[verticeIndex].y,
						mesh->mTangents[verticeIndex].z);
					/*tangent = tangent * CU::Math::CreateReflectionMatrixAboutAxis(CU::Vector3f(-1, 0, 0));
					CU::Math::Normalize(tangent);*/


					data->myVertexBuffer[currIndex + addedSize] = tangent.x;
					data->myVertexBuffer[currIndex + addedSize + 1] = tangent.y;
					data->myVertexBuffer[currIndex + addedSize + 2] = tangent.z;
					data->myVertexBuffer[currIndex + addedSize + 3] = 0;
					addedSize += TANGENT_STRIDE;
				}

				indices.Add(verticeIndex);
				vertCount++;
			}
		}

	/*	data->m_MinPoint = min_point;
		data->m_MaxPoint = max_point;*/
	}


	const u32 indice_byte_width = sizeof(u32)* polygonCount * 3;
	data->myIndicies = new s8[indice_byte_width];
	ZeroMemory(data->myIndicies, indice_byte_width);
	data->m_IndexBufferSize = indice_byte_width;

	////Flips it to make it correct.
	//CU::GrowingArray<u32> indiceFix(indices.Size());
	//for ( s32 indice = indices.Size() - 1; indice >= 0; indice-- )
	//{
	//	indiceFix.Add(indices[indice]);
	//}

	memcpy(data->myIndicies, &indices[0], sizeof(u32) * indices.Size());

	data->myVertexStride = stride * sizeof(float);
	data->myVertexCount = vertCount;
	data->myIndexCount = indices.Size();


	ExtractMaterials(mesh, scene, data, file);

	fbx->m_Data = data;

}

void CModelImporter::ExtractMaterials(aiMesh* mesh, const aiScene* scene, ModelData* data, std::string file)
{

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	for ( u32 i = 0; i < material->mNumProperties; i++ )
	{
		aiMaterialProperty* prop = material->mProperties[i];
		u32 type = prop->mSemantic;
		aiString str;
		material->GetTexture(static_cast< aiTextureType >( type ), 0, &str);
		std::string path = cl::substr(file, "/", true, 0);

		std::string fileName = cl::substr(str.C_Str(), "\\", false, 0);
		if ( fileName.empty() )
			fileName = cl::substr(str.C_Str(), "/", false, 0);
		if ( cl::substr(str.C_Str(), "\\") )
		{
			fileName.erase(0, 1);
		}
		path += "/";
		path += fileName;

		if ( fileName != "" )
		{
			TextureInfo newInfo;
			newInfo.m_File = path;
			aiTextureType lType = static_cast< aiTextureType >( type );
			switch ( lType )
			{
				case aiTextureType_DIFFUSE:
				{
					newInfo.m_Slot = Effect::DIFFUSE;
				}break;

				case aiTextureType_NORMALS:
				{
					newInfo.m_Slot = Effect::NORMAL;
				}break;

				case aiTextureType_EMISSIVE:
				{
					newInfo.m_Slot = Effect::EMISSIVE;
				}break;

				case aiTextureType_AMBIENT:
				{
					newInfo.m_Slot = Effect::AO;
				}break;

				case aiTextureType_OPACITY:
				{
					newInfo.m_Slot = Effect::OPACITY;
				}break;

				case aiTextureType_REFLECTION: /* Misleading name, amirite? */
				{
					newInfo.m_Slot = Effect::METALNESS; // specular intensity (blender), metalness 
				}break;

				case aiTextureType_SPECULAR:
				{
					newInfo.m_Slot = Effect::METALNESS; // Blender <- Roughness, Maya <- Metal
				}break;

				case aiTextureType_HEIGHT:
				{
					DL_ASSERT("No height support!");
				}break;

				case aiTextureType_SHININESS:
				{
					newInfo.m_Slot = Effect::ROUGHNESS; // specular hardness (blender)
				}break;

				case aiTextureType_DISPLACEMENT:
				{
					DL_ASSERT("No displacement support");
					//newInfo.m_Slot = TextureType::_DISPLACEMENT;
				}break;

				case aiTextureType_LIGHTMAP:
				{
					DL_ASSERT("No lightmap support");
					//newInfo.m_Slot =Effect;
				}break;

				case aiTextureType_UNKNOWN:
				{
					DL_ASSERT("Not implemented");
				}break;
			}

			bool found = false;


			const CU::GrowingArray<TextureInfo> texInfo = data->myTextures;
			for ( const TextureInfo& info : texInfo )
			{
				if ( info.m_File == newInfo.m_File && info.m_File == newInfo.m_File )
				{
					found = true;
					break;
				}
			}

			if ( found == false )
			{
				data->myTextures.Add(newInfo);
			}

		}
	}
}
