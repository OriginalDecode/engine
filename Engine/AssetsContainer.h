#pragma once
#include <map>
#include <unordered_map>
#include "engine_shared.h"
#include <DataStructures/GrowingArray.h>
#include <Engine/ModelImporter.h>
#include <CommonLib/DataStructures/StaticArray.h>s

class FileWatcher;
class ShaderFactory;
class Model;
class Effect;
class Texture;
class Sprite;

struct CompiledShader;

class AssetsContainer
{
public:
	AssetsContainer() = default;
	~AssetsContainer();
	void Initiate();

	void Update();

	void ReloadTexture(Texture* texture);
	Texture* GetTexture(std::string aFilePath);

	Sprite* GetSprite(const cl::CHashString<128>& path);
	Effect* GetEffect(const std::string& aFilePath);
	Model* GetModel(const std::string& aFilePath);
	std::string LoadModel(std::string aFilePath, std::string effect, bool thread = true);
	template<typename T>
	std::string LoadModel(std::string filepath, std::string effect_filepath, T* pModel, bool thread = true);


private:
#ifndef FINAL
	CU::StaticArray<FileWatcher*, 4> m_Watchers;
#endif


	Ticket_Mutex m_Mutex;
	Ticket_Mutex m_GetModelMutex;
	std::unordered_map<std::string, Texture*> myTextures;
	std::unordered_map<std::string, Effect*> myEffects;
	std::unordered_map<std::string, Model*> myModels;
	std::map<u64, Sprite*> m_Sprites;

	bool LoadTexture(std::string aFilePath);
	Effect* LoadEffect(const std::string& aFilePath);

	Sprite* LoadSprite(const cl::CHashString<128>& path);

	ShaderFactory* m_ShaderFactory;
	CModelImporter* m_ModelLoader;

};

template<typename T>
std::string AssetsContainer::LoadModel(std::string filepath, std::string effect_filepath, T* pModel, bool thread /*= true*/)
{
	if (myModels.find(filepath) != myModels.end())
		return filepath;
	DL_MESSAGE("Loading model : %s", filepath.c_str());
	T* model = new T;
	myModels.emplace(filepath, model);

	if (thread)
	{
		Engine::GetInstance()->GetThreadpool().AddWork(Work([=]() {
			m_ModelLoader->LoadModel(model, filepath, effect_filepath);
			model->Initiate(filepath);
		}));
	}
	else
	{
		m_ModelLoader->LoadModel(model, filepath, effect_filepath);
		model->Initiate(filepath);
	}

	return filepath;

}
