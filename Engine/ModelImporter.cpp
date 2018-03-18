#include "stdafx.h"
#include "ModelImporter.h"


#include <Engine/Model.h>


CModelImporter::CModelImporter()
	: m_Engine(Engine::GetInstance())
{
}

void CModelImporter::ExtractMaterials(aiMesh* mesh, const aiScene* scene, ModelData& data, std::string file)
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


		if (fileName.find("curtain") != fileName.npos || fileName.find("Curtain") != fileName.npos)
		{
			int apa;
			apa = 5;
		}

		if ( fileName != "" )
		{
			TextureInfo newInfo;
			newInfo.m_File = fileName;
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

			bool _found = false;


			const CU::GrowingArray<TextureInfo> texInfo = data.myTextures;
			for ( const TextureInfo& info : texInfo )
			{
				if ( info.m_File == newInfo.m_File )
				{
					_found = true;
					break;
				}
			}

			if (_found == false )
			{
				data.myTextures.Add(newInfo);
			}

		}
	}
}
