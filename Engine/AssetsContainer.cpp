#include "stdafx.h"
#include "AssetsContainer.h"
#include "Model.h"
#include "Texture.h"
#include "Effect.h"
#include "ShaderFactory.h"
#include "ModelImporter.h"
#include "Texture.h"


AssetsContainer::~AssetsContainer()
{
	SAFE_DELETE(m_ShaderFactory);
	SAFE_DELETE(m_ModelLoader);
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

void AssetsContainer::Initiate()
{
	m_Engine = Engine::GetInstance();
	m_ShaderFactory = new ShaderFactory;
	m_ModelLoader = new CModelImporter;
	myTextures.empty();
	myEffects.empty();
	myModels.empty();
	mySprites.empty();
}

Texture* AssetsContainer::GetTexture(const std::string& aFilePath)
{
	if (CL::substr(aFilePath, ".dds") == false)
	{
		DL_MESSAGE("Failed to load %s, due to incorrect fileformat. Has to be .dds", aFilePath.c_str());
		DL_ASSERT("Failed to Load Texture, format not .dds. See log for more information.");
	}


	if (myTextures.find(aFilePath) == myTextures.end())
	{
		if ( !LoadTexture(aFilePath) )
		{
			DL_ASSERT("Failed to load %s, an unexpected error occured. Concurrency issue.", aFilePath.c_str());
			return nullptr;
		}
		DL_MESSAGE("Successfully loaded : %s", aFilePath.c_str());
	}

	return myTextures[aFilePath];
}

Effect* AssetsContainer::GetEffect(const std::string& aFilePath)
{
	if (myEffects.find(aFilePath) == myEffects.end())
	{
		LoadEffect(aFilePath);
	}
	return myEffects[aFilePath];
}

CModel* AssetsContainer::GetModel(const std::string& aFilePath)
{
	if (myModels.find(aFilePath) == myModels.end())
	{
		DL_MESSAGE("Requested Model : %s", aFilePath.c_str());
		DL_ASSERT("Failed to find requested model. Did you enter the correct path?");
	}
	return myModels[aFilePath];
}

void AssetsContainer::Update()
{
	m_ShaderFactory->Update();
}

void AssetsContainer::ReloadTexture(Texture* texture)
{
	texture->OnReload();
}

bool AssetsContainer::LoadTexture(const std::string& aFilePath)
{
	if (myTextures.find(aFilePath) == myTextures.end())
	{
		myTextures.emplace(aFilePath, new Texture);
		Texture* texture = myTextures.find(aFilePath)->second;
		if (texture->Load(aFilePath.c_str()) == false)
		{
			DL_ASSERT_EXP(texture->CleanUp(), "Failed to cleanup texture!");
			SAFE_DELETE(texture);
			return false;
		}
	}

	return true;
}

void AssetsContainer::LoadEffect(const std::string& aFilePath)
{
	Effect* effect = new Effect(aFilePath);
	m_ShaderFactory->LoadShader(effect);
	myEffects[aFilePath] = effect;
}

#define THREAD_LOADING

std::string AssetsContainer::LoadModel(std::string aFilePath, std::string effect)
{
	if (myModels.find(aFilePath) == myModels.end())
	{
		ENGINE_LOG("Loading model : %s", aFilePath.c_str());
		CModel* model = new CModel;
		myModels.emplace(aFilePath, model);
		std::string file_path;

#ifdef THREAD_LOADING
		m_Engine->GetThreadpool().AddWork(Work( [=] () {
			myModels[aFilePath] = m_ModelLoader->LoadModel(aFilePath, model, effect);
		}));
#else
		myModels[aFilePath] = m_ModelLoader->LoadModel(aFilePath, model, effect);
#endif
	}
	return aFilePath;
}

void AssetsContainer::AddLoadRequest(std::string file, eRequestType request_type)
{
	BeginTicketMutex(&m_Mutex);

	bool already_exist = false;
	for (const LoadRequest& request : m_RequestList)
	{
		if (request.m_File == file)
		{
			already_exist = true;
		}
	}


	if (!already_exist)
	{
		LoadRequest request(request_type, file);
		m_RequestList.Add(request);
	}

	EndTicketMutex(&m_Mutex);
}
