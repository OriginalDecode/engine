#pragma once
#include <map>
#include <unordered_map>
#include "engine_shared.h"
#include <CommonLib/DataStructures/GrowingArray.h>
#include <Engine/ModelImporter.h>
#include <CommonLib/DataStructures/StaticArray.h>
#include <Engine/DebugHandle.h>
#include <CommonLib/RefPointer.h>


class FileWatcher;
class ShaderFactory;
class Model;
class Effect;
class Texture;
class Sprite;
class Material;


struct CompiledShader;
class AssetsContainer
{
#ifdef _DEBUG
	friend debug::DebugHandle;
#endif
public:

	static AssetsContainer* GetInstance() 
	{ 
		static AssetsContainer * instance = nullptr;
		if (!instance)
			instance = new AssetsContainer;

		return instance; 
	}

	void Initiate();

	void Update();

	void ReloadTexture(Texture* texture);


	void AddTexture(Texture* pTexture, uint64 key);

	Texture* GetTexture(uint64 key);
	Sprite* GetSprite(uint64 key);
	Effect* GetEffect(uint64 key);
	Material* GetMaterial(uint64 key);

	template<typename T>
	RefPointer<T> GetModel(uint64 key);

	Model* GetModelDirect(uint64 key);

	//u64 GetModelKey(const char* path) const;


	template<typename T>
	HashType LoadModel(std::string path, std::string effect_filepath, bool thread = true, int option = 0);

	uint64 LoadTexture(std::string path, bool make_mips = false);
	uint64 LoadEffect(std::string path);
	uint64 LoadSprite(std::string path);
	uint64 LoadMaterial(std::string path);

	uint64 LoadShader(std::string path, std::string entrypoint);


	CompiledShader* GetShader(const char* key) const;
	CompiledShader* GetShader(const uint64 key) const;

	ShaderFactory* GetShaderFactory();
private:

	AssetsContainer() = default;
	~AssetsContainer();

#ifndef FINAL
	FileWatcher* m_Watcher = nullptr;
#endif


	std::map<uint64, Texture*> m_Textures;
	std::map<uint64, Effect*> m_Effects;
	std::map<HashType, RefPointer<Model>> m_Models;
	std::map<uint64, Sprite*> m_Sprites;
	std::map<uint64, Material*> m_Materials;




	ShaderFactory* m_ShaderFactory;

};

template<typename T>
HashType AssetsContainer::LoadModel(std::string path, std::string effect_filepath, bool thread /*= true*/, int option)
{
	if (!cl::file_exist(path) && path.find("default") != 0)
		DL_ASSERT("Failed to find the file!");

	
	HashType hash;
	hash.m_Hash = cl::Hash(path.c_str());

	Engine* engine = Engine::GetInstance();
	if (path.find("default") != path.npos)
	{
		hash.m_Hash = cl::Hash("default");
		return hash;
	}

	if (m_Models.find(hash) != m_Models.end())
		return hash;

	Effect* effect = engine->GetEffect(effect_filepath.c_str());

	m_Models.emplace(hash, RefPointer<Model>(new T));
	T* model = static_cast<T*>(m_Models.at(hash).GetData());
	
	if (thread)
	{
		engine->GetThreadpool().AddWork(Work([=]() {
			CModelImporter importer;
			importer.LoadModel<T>(path, model, effect, option);
			model->Initiate(path);
		}));
	}
	else
	{
		CModelImporter importer;
		importer.LoadModel<T>(path, model, effect, option);
		model->Initiate(path);
	}

	model->SetEntityID(hash);

	return hash;
}

template<typename T>
RefPointer<T> AssetsContainer::GetModel(uint64 key)
{
	auto it = m_Models.find(key);

	if (it != m_Models.end())
		return it->second;

	return nullptr;
}