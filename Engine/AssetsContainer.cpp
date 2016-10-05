#include "stdafx.h"
#include "AssetsContainer.h"
#include "Model.h"
#include "Texture.h"
#include "Effect.h"
#include "ShaderFactory.h"
#include "ModelImporter.h"
namespace Snowblind
{
	CAssetsContainer::CAssetsContainer() 
		: myModelLoader(new CModelImporter())
	{
	}

	CAssetsContainer::~CAssetsContainer()
	{
		SAFE_DELETE(myShaderFactory);
		DELETE_MAP(myModels);
		DELETE_MAP(myEffects);
		DELETE_MAP(myTextures);
	}

	void CAssetsContainer::Initiate()
	{
		myShaderFactory = new CShaderFactory();
	}

	CTexture* CAssetsContainer::GetTexture(const std::string& aFilePath)
	{
		if (CL::substr(aFilePath, ".dds") == false)
		{
			DL_MESSAGE("Failed to load %s, due to incorrect fileformat. Has to be .dds", aFilePath.c_str());
			DL_ASSERT("Failed to Load Texture, format not .dds. See log for more information.");
		}

		if (myTextures.find(aFilePath) == myTextures.end())
		{
			LoadTexture(aFilePath);
			DL_MESSAGE("Successfully loaded : %s", aFilePath.c_str());
		}

		return myTextures[aFilePath];
	}

	CEffect* CAssetsContainer::GetEffect(const std::string& aFilePath)
	{
		if (myEffects.find(aFilePath) == myEffects.end())
		{
			LoadEffect(aFilePath);
		}
		return myEffects[aFilePath];
	}

	CModel* CAssetsContainer::GetModel(const std::string& aFilePath)
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

	void CAssetsContainer::LoadTexture(const std::string& aFilePath)
	{
		if (myTextures.find(aFilePath) == myTextures.end())
		{
			CTexture* texture = new CTexture();
			if (texture->LoadTexture(aFilePath.c_str()) == false)
			{
				SAFE_DELETE(texture);
			}
			myTextures[aFilePath] = texture;
		}
	}

	void CAssetsContainer::LoadEffect(const std::string& aFilePath)
	{
		CEffect* effect = new CEffect(aFilePath);
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