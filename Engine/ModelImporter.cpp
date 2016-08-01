#include "stdafx.h"
#include "ModelImporter.h"
#include <DL_Debug.h>
#include "Model.h"

#include "VertexWrapper.h"
#include "IndexWrapper.h"

CModelImporter::CModelImporter()
	: myEngine(Snowblind::CEngine::GetInstance())
	, myTimeManager(new CommonUtilities::TimeManager())
{
	myTimeManager->CreateTimer();
}


CModelImporter::~CModelImporter()
{
	SAFE_DELETE(myTimeManager);
}

Snowblind::CModel* CModelImporter::CreateModel(/*Data*/ Snowblind::CEffect* anEffect)
{
	Snowblind::CModel* newModel = new Snowblind::CModel();
	newModel->SetEffect(anEffect);

	if (/*data*/0)
	{
		/* FillData */
	}

	for (u32 i = 0; i < 0 /*Data->ChildCount*/; i++)
	{
		auto currentChild = 0;
		newModel->AddChild(CreateModel(anEffect));
	}

}

Snowblind::CModel* CModelImporter::LoadModel(const s8* aFilePath, const s8* aEffectPath)
{
	return LoadModel(aFilePath, myEngine->GetEffect(aEffectPath));
}

Snowblind::CModel* CModelImporter::LoadModel(const s8* aFilePath, Snowblind::CEffect* anEffect)
{
	myTimeManager->Update();
	float loadTime = myTimeManager->GetTimer(0).GetTotalTime().GetMilliseconds();

	/*
		Check if data exists.

		struct
		ModelData = LoadModelData(data)
		FilePath

		if exists set modeldata to data that is found

		else create new fbx data, add it to the data array and set the modelData to the new data.
	*/

	Snowblind::CModel* toReturn = nullptr;
	loadTime = myTimeManager->GetTimer(0).GetTotalTime().GetMilliseconds() - loadTime;
	MODEL_LOG("%s took %fms to load. %s", aFilePath, loadTime, (loadTime < 7000.f) ? "" : "Check if it's saved as binary.");
	return toReturn;
}

void CModelImporter::ImportModel(const std::string& aFilepath)
{
	const aiScene* scene = importer.ReadFile(aFilepath, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_MakeLeftHanded); //MaxQuality, Quality, Fast
	DL_MESSAGE("Requested Model : %s", aFilepath.c_str());
	DL_MESSAGE_EXP(!scene, "%s", importer.GetErrorString());
	DL_ASSERT_EXP(scene, "ImportModel Failed. Could not read the requested file.");


	aiNode* root = scene->mRootNode;
	u32 childCount = root->mNumChildren;
	u32 texCount = scene->mNumTextures;

	if (scene)
	{


	}
	for (u32 i = 0; i < childCount; i++)
	{
		aiNode* c = root->mChildren[i];
		u32 meshCount = c->mNumMeshes;
		
		for (u32 m = 0; m < meshCount; m++)
		{
			CU::GrowingArray<aiMaterialProperty*> props;
			aiMesh* mesh = scene->mMeshes[c->mMeshes[m]];
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			//Do a check on the texture path string.
			u32 matCount = material->mNumProperties;

			//this data are supposed to go on the surface.
			for (u32 j = 0; j < matCount; j++)
			{
				aiMaterialProperty* prop = material->mProperties[j];

				u32 type = prop->mSemantic;

				aiString str;
				material->GetTexture(static_cast<aiTextureType>(type), 0, &str);

				std::string newPath = CL::substr(aFilepath, "/", true, 0);
				std::string fileName = CL::substr(str.C_Str(), "\\", false, 0);
				fileName.erase(0, 1);
				newPath += "/";
				newPath += fileName;
				if (fileName != "")
					myEngine->GetTexture(newPath); //All textures now get properly loaded.

			}
		}
	}

}

void CModelImporter::FillData(Snowblind::CModel* out, Snowblind::CEffect* anEffect)
{

}
