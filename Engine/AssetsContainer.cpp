#include "stdafx.h"
#include "AssetsContainer.h"
#include "Model.h"
#include "Texture.h"
#include "Effect.h"
#include "FBXFactory.h"
#include "ShaderFactory.h"
namespace Snowblind
{
	CAssetsContainer* CAssetsContainer::myInstance = nullptr;

	CAssetsContainer::CAssetsContainer()
	{
		myFactory = new FBXFactory();
		myShaderFactory = new CShaderFactory();
	}

	CAssetsContainer::~CAssetsContainer()
	{
		SAFE_DELETE(myFactory);
		SAFE_DELETE(myShaderFactory);

		for (auto it = myModels.begin(); it != myModels.end(); ++it)
		{
			SAFE_DELETE(it->second);
		}

		for (auto it = myEffects.begin(); it != myEffects.end(); ++it)
		{
			SAFE_DELETE(it->second);
		}

		for (auto it = myTextures.begin(); it != myTextures.end(); ++it)
		{
			SAFE_DELETE(it->second);
		}


	}

	void CAssetsContainer::Create()
	{
		DL_ASSERT_EXP(myInstance == nullptr, "Can't create AssetContainer twice.");
		myInstance = new CAssetsContainer();
	}

	void CAssetsContainer::Destroy()
	{
		DL_ASSERT_EXP(myInstance != nullptr, "Can't remove AssetContainer, object did not exsist.");
		SAFE_DELETE(myInstance);
	}

	CAssetsContainer* CAssetsContainer::GetInstance()
	{
		DL_ASSERT_EXP(myInstance != nullptr, "Can't get AssetContainer, object did not exsist.");
		return myInstance;
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

	CModel* CAssetsContainer::GetModel(const std::string& aFilePath, const std::string& effect)
	{
		if (myModels.find(aFilePath) == myModels.end())
		{
			LoadModel(aFilePath, effect);
		}
		return myModels[aFilePath];
	}

	void CAssetsContainer::Update()
	{
		myShaderFactory->Update();
	}

	void CAssetsContainer::LoadTexture(const std::string& aFilePath)
	{
		CTexture* texture = new CTexture();
		if (texture->LoadTexture(aFilePath.c_str()) == false)
		{
			SAFE_DELETE(texture);
		}
		myTextures[aFilePath] = texture;
	}

	void CAssetsContainer::LoadEffect(const std::string& aFilePath)
	{
		CEffect* effect = new CEffect(aFilePath);

		myShaderFactory->LoadShader(effect);

		myEffects[aFilePath] = effect;
	}

	void CAssetsContainer::LoadModel(const std::string& aFilePath, const std::string& effect)
	{
		CModel* model = myFactory->LoadModel(aFilePath.c_str(), effect.c_str());
		model->CreateModel();
		myModels[aFilePath] = model;
	}
};