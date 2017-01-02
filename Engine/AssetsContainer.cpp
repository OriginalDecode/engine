#include "stdafx.h"
#include "AssetsContainer.h"
#include "Model.h"
#include "Texture.h"
#include "Effect.h"
#include "ShaderFactory.h"
#include "ModelImporter.h"
#include "Texture.h"

namespace Cache
{
	CAssetsContainer::~CAssetsContainer()
	{
		SAFE_DELETE(myShaderFactory);
		SAFE_DELETE(myModelLoader);
		for (auto it = myModels.begin(); it != myModels.end(); it++)
		{
			DL_ASSERT_EXP(it->second->CleanUp(), "Failed to cleanup a model.");
			SAFE_DELETE(it->second);
		}

		for (auto it = myTextures.begin(); it != myTextures.end(); it++)
		{
			DL_ASSERT_EXP(it->second->CleanUp(), "Failed to cleanup a model.");
			SAFE_DELETE(it->second);
		}

		DELETE_MAP(myEffects);
	}

	void CAssetsContainer::Initiate()
	{
		myShaderFactory = new Hex::ShaderFactory;
		myModelLoader = new CModelImporter;
	}

	Hex::Texture* CAssetsContainer::GetTexture(const std::string& aFilePath)
	{
		if (CL::substr(aFilePath, ".dds") == false)
		{
			DL_MESSAGE("Failed to load %s, due to incorrect fileformat. Has to be .dds", aFilePath.c_str());
			DL_ASSERT("Failed to Load Texture, format not .dds. See log for more information.");
		}

		if (myTextures.find(aFilePath) == myTextures.end())
		{
			if (!LoadTexture(aFilePath))
				return nullptr;
			DL_MESSAGE("Successfully loaded : %s", aFilePath.c_str());
		}

		return myTextures[aFilePath];
	}

	Hex::Effect* CAssetsContainer::GetEffect(const std::string& aFilePath)
	{
		if (myEffects.find(aFilePath) == myEffects.end())
		{
			LoadEffect(aFilePath);
		}
		return myEffects[aFilePath];
	}

	Hex::CModel* CAssetsContainer::GetModel(const std::string& aFilePath)
	{
		if (myModels.find(aFilePath) == myModels.end())
		{
			DL_MESSAGE("Requested Model : %s", aFilePath.c_str());
			DL_ASSERT("Failed to find requested model. Did you enter the correct path?");
		}
		return myModels[aFilePath];
	}

	void CAssetsContainer::Update()
	{
		myShaderFactory->Update();
	}

	void CAssetsContainer::ReloadTexture(Hex::Texture* texture)
	{
		texture->OnReload();
	}

	bool CAssetsContainer::LoadTexture(const std::string& aFilePath)
	{
		if (myTextures.find(aFilePath) == myTextures.end())
		{
			Hex::Texture* texture = new Hex::Texture;
			if (texture->Load(aFilePath.c_str()) == false)
			{
				DL_ASSERT_EXP(texture->CleanUp(), "Failed to cleanup texture!");
				SAFE_DELETE(texture);
				return false;
			}
			myTextures[aFilePath] = texture;
		}
		return true;
	}

	void CAssetsContainer::LoadEffect(const std::string& aFilePath)
	{
		Hex::Effect* effect = new Hex::Effect(aFilePath);
		myShaderFactory->LoadShader(effect);
		myEffects[aFilePath] = effect;
	}

	const std::string& CAssetsContainer::LoadModel(const std::string& aFilePath, const std::string& effect)
	{
		if (myModels.find(aFilePath) == myModels.end())
		{
			myModels[aFilePath] = myModelLoader->LoadModel(aFilePath, effect);
		}

		return aFilePath;
	}
};