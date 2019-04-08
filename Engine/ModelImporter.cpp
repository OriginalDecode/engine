#include "stdafx.h"
#include "ModelImporter.h"


#include <Engine/Model.h>

void CModelImporter::AddVertexNormalData(uint32 &stride, ModelData &data, uint32 &size, uint32 polygonVertexCount)
{
	ModelData::Layout newLayout;
	newLayout.myType = ModelData::VERTEX_NORMAL;
	newLayout.mySize = NORMAL_STRIDE;
	newLayout.myOffset = stride * 4;
	data.myLayout.Add(newLayout);

	stride += NORMAL_STRIDE;
	size += polygonVertexCount * NORMAL_STRIDE;
}

void CModelImporter::AddUVData(uint32 &stride, ModelData &data, uint32 &size, uint32 polygonVertexCount)
{
	ModelData::Layout newLayout;
	newLayout.myType = ModelData::VERTEX_UV;
	newLayout.mySize = UV_STRIDE;
	newLayout.myOffset = stride * 4;
	data.myLayout.Add(newLayout);

	stride += UV_STRIDE;
	size += polygonVertexCount * UV_STRIDE;
}

void CModelImporter::AddTangentData(uint32 &stride, ModelData &data, uint32 &size, uint32 polygonVertexCount)
{
	ModelData::Layout newLayout;
	newLayout.myType = ModelData::VERTEX_TANGENT;
	newLayout.mySize = TANGENT_STRIDE;
	newLayout.myOffset = stride * 4;
	data.myLayout.Add(newLayout);

	stride += TANGENT_STRIDE;
	size += polygonVertexCount * TANGENT_STRIDE;
}

void CModelImporter::AddBiNormalData(uint32 &stride, ModelData &data, uint32 &size, uint32 polygonVertexCount)
{
	ModelData::Layout newLayout;
	newLayout.myType = ModelData::VERTEX_BINORMAL;
	newLayout.mySize = BINORMAL_STRIDE;
	newLayout.myOffset = stride * 4;
	data.myLayout.Add(newLayout);

	stride += BINORMAL_STRIDE;
	size += polygonVertexCount * BINORMAL_STRIDE;
}

void CModelImporter::ExtractMaterials(aiMesh* mesh, const aiScene* scene, ModelData& data, std::string file)
{
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	for (uint32 i = 0; i < material->mNumProperties; i++)
	{
		aiMaterialProperty* prop = material->mProperties[i];
		uint32 type = prop->mSemantic;
		aiString texture_file;
		material->GetTexture(static_cast<aiTextureType>(type), 0, &texture_file);

		assert(texture_file.length >= 0 && "Extracted a material with a string that was less than 0, data corruption?");
		if (texture_file.length > 0)
		{

			std::string texture_info(texture_file.C_Str());
			const size_t pos = texture_info.rfind("\\");
			texture_info = texture_info.substr(pos + 1);

			TextureInfo newInfo;
			newInfo.m_File = texture_info;
			aiTextureType lType = static_cast<aiTextureType>(type);
			switch (lType)
			{
				case aiTextureType_DIFFUSE:
				{
					newInfo.m_Slot = TextureSlot::DIFFUSE;
				}break;

				case aiTextureType_NORMALS:
				{
					newInfo.m_Slot = TextureSlot::NORMAL;
				}break;

				case aiTextureType_EMISSIVE:
				{
					newInfo.m_Slot = TextureSlot::EMISSIVE;
				}break;

				case aiTextureType_AMBIENT:
				{
					newInfo.m_Slot = TextureSlot::AO;
				}break;

				case aiTextureType_OPACITY:
				{
					newInfo.m_Slot = TextureSlot::OPACITY;
				}break;

				case aiTextureType_REFLECTION: /* Misleading name, amirite? */
				{
					newInfo.m_Slot = TextureSlot::METALNESS; // specular intensity (blender), metalness 
				}break;

				case aiTextureType_SPECULAR:
				{
					newInfo.m_Slot = TextureSlot::METALNESS; // Blender <- Roughness, Maya <- Metal
				}break;

				case aiTextureType_HEIGHT:
				{
					DL_MESSAGE("Height Support!");
					//DL_ASSERT("No height support!");
				}break;

				case aiTextureType_SHININESS:
				{
					newInfo.m_Slot = TextureSlot::ROUGHNESS; // specular hardness (blender)
				}break;

				case aiTextureType_DISPLACEMENT:
				{
					//DL_ASSERT("No displacement support");
					DL_MESSAGE("No displacement support");
					//newInfo.m_Slot = TextureType::_DISPLACEMENT;
				}break;

				case aiTextureType_LIGHTMAP:
				{
					//DL_ASSERT("No lightmap support");
					DL_MESSAGE("No lightmap support");
					//newInfo.m_Slot =Effect;
				}break;

				case aiTextureType_UNKNOWN:
				{
					//DL_ASSERT("Not implemented");
					DL_MESSAGE("Not implemented");
				}break;
			}

			bool _found = false;


			const CU::GrowingArray<TextureInfo>& texInfo = data.myTextures;
			for (const TextureInfo& info : texInfo)
			{
				if (info.m_File == newInfo.m_File)
				{
					_found = true;
					break;
				}
			}

			if (_found == false)
			{
				data.myTextures.Add(newInfo);
			}

		}
	}
}

void CModelImporter::AddVertexPositionData(uint32 &stride, ModelData &data, uint32 &size, uint32 polygonVertexCount)
{
	ModelData::Layout newLayout;
	newLayout.myType = ModelData::VERTEX_POS;
	newLayout.mySize = VERTEX_STRIDE;
	newLayout.myOffset = 0;
	data.myLayout.Add(newLayout);
	size += polygonVertexCount * VERTEX_STRIDE;
	stride += VERTEX_STRIDE;
}

void CModelImporter::AddBoneData(uint32& stride, ModelData& data, uint32& size, uint32 polygonVertexCount)
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