#pragma once
#include <map>
#include <unordered_map>
#include "snowblind_shared.h"
#include <DataStructures/GrowingArray.h>
class CModelImporter;
class FileWatcher;
class ShaderFactory;
class Model;
class Effect;
class Texture;
class Sprite;
struct CompiledShader;
class Engine;
class AssetsContainer
{
public:

	enum eRequestType
	{
		MODEL,
		TEXTURE,
		SHADER,
		SPRITE,
	};


	AssetsContainer() = default;
	~AssetsContainer();

	void Initiate();

	void Update();

	void ReloadTexture(Texture* texture);
	Texture* GetTexture(std::string aFilePath);

	Effect* GetEffect(const std::string& aFilePath);
	Model* GetModel(const std::string& aFilePath);
	std::string LoadModel(std::string aFilePath, std::string effect, bool thread = true);
	template<typename T>
	std::string LoadModel(std::string filepath, std::string effect_filepath, T* pModel, bool thread = true);

	void AddLoadRequest(std::string file, eRequestType request_type);

private:
	Engine* m_Engine = nullptr;
	FileWatcher* m_TextureWatcher = nullptr;
	Ticket_Mutex m_Mutex;
	std::unordered_map<std::string, Texture*> myTextures;
	std::unordered_map<std::string, Effect*> myEffects;
	std::unordered_map<std::string, Model*> myModels;
	std::unordered_map<std::string, Sprite*> mySprites;

	bool LoadTexture(std::string aFilePath);
	void LoadEffect(const std::string& aFilePath);

	ShaderFactory* m_ShaderFactory;
	CModelImporter* m_ModelLoader;

	struct LoadRequest
	{
		LoadRequest() = default;
		LoadRequest(eRequestType request_type, std::string file) 
			: m_Type(request_type)
			, m_File(file)
		{}
		eRequestType m_Type;
		std::string m_File;
	};

	CU::GrowingArray<LoadRequest> m_RequestList;

};

template<typename T>
std::string AssetsContainer::LoadModel(std::string filepath, std::string effect_filepath, T* pModel, bool thread /*= true*/)
{
	if (myModels.find(filepath) != myModels.end())
		return filepath;
	DL_MESSAGE("Loading model : %s", filepath.c_str());

	myModels.emplace(filepath, pModel);

	if (thread)
	{
		m_Engine->GetThreadpool().AddWork(Work([=]() {
			m_ModelLoader->LoadModel(filepath, effect_filepath, pModel);
		}));
	}
	else
	{
		m_ModelLoader->LoadModel(filepath, effect_filepath, pModel);
	}

	return filepath;

}
