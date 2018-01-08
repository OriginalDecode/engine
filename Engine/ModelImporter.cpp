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

void CModelImporter::ProcessNode(aiNode* aNode, const aiScene* aScene, FBXModelData* someData, std::string file, CU::Vector3f& min_point, CU::Vector3f& max_point)
{
	DL_ASSERT_EXP(someData, "Failed to process node. FBXModelData someData was null");

	for ( u32 i = 0; i < aNode->mNumMeshes; i++ )
	{
		aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
		ProcessMesh(mesh, aScene, someData, file, min_point, max_point);
		DL_ASSERT_EXP(someData->myData, "Was null after ProcessMesh!?");
	}

	for ( u32 i = 0; i < aNode->mNumChildren; i++ )
	{
		someData->myChildren.Add(new FBXModelData);
		ProcessNode(aNode->mChildren[i], aScene, someData->myChildren.GetLast(), file, min_point, max_point);
	}
}


void CModelImporter::ProcessMesh(aiMesh* mesh, const aiScene* scene, FBXModelData* fbx, std::string file, CU::Vector3f& min_point, CU::Vector3f& max_point)
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

	if ( mesh->HasTextureCoords(0) ) //this is multiple coords D:
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

	//if (mesh->HasVertexColors(0))
	//{
	//	mesh->mColors
	//}


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
	data->myData->m_VertexBufferSize = sizeof(float) * size;
	DL_ASSERT_EXP(mesh->mNumVertices < size, "the amount of vertices was MORE!? than size");
	data->myData->myIndicies = new u32[polygonCount * 3];
	ZeroMemory(data->myData->myIndicies, sizeof(u32) * polygonCount * 3);
	data->myData->m_IndexBufferSize = sizeof(u32) * polygonCount * 3;
	CU::GrowingArray<u32> indices(polygonCount * 3);
	u32 vertCount = 0;

	//CU::Vector3f min_point, max_point;

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
				data->myData->myVertexBuffer[currIndex + 3] = 1;


				if ( i != 0 )
				{

					min_point.x = min(position.x, min_point.x);
					min_point.y = min(position.y, min_point.y);
					min_point.z = min(position.z, min_point.z);

					max_point.x = max(position.x, max_point.x);
					max_point.y = max(position.y, max_point.y);
					max_point.z = max(position.z, max_point.z);

					/*if ( position.x <= min_point.x )
						min_point.x = position.x;

					if ( position.x > max_point.x )
						data->myData->m_MaxPoint.x = position.x;

					if ( position.y <= data->myData->m_MinPoint.y )
						data->myData->m_MinPoint.y = position.y;

					if ( position.y > data->myData->m_MaxPoint.y )
						data->myData->m_MaxPoint.y = position.y;

					if ( position.z <= data->myData->m_MinPoint.z )
						data->myData->m_MinPoint.z = position.z;

					if ( position.z > data->myData->m_MaxPoint.z )
						data->myData->m_MaxPoint.z = position.z;*/
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
	}

	//Flips it to make it correct.
	CU::GrowingArray<u32> indiceFix(indices.Capacity());
	for ( s32 indice = indices.Size() - 1; indice >= 0; indice-- )
	{
		indiceFix.Add(indices[indice]);
	}

	memcpy(data->myData->myIndicies, &indiceFix[0], sizeof(u32) * indiceFix.Size());

	data->myData->myVertexStride = stride * sizeof(float);
	data->myData->myVertexCount = vertCount;
	data->myData->myIndexCount = indiceFix.Size();


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
					newInfo.m_Slot = Effect::ROUGHNESS; // specular intensity (blender), roughness???
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


			const CU::GrowingArray<TextureInfo> texInfo = data->myTextureData->myTextures;
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
				data->myTextureData->myTextures.Add(newInfo);
			}

		}
	}
}

void CModelImporter::FBXModelData::DeleteChildren()
{
	for (FBXModelData* child : myChildren)
	{
		child->DeleteChildren();
		delete child->myData;
		delete child->myTextureData;
	}
}
