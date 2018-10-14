#pragma once
#include <map>
#include <unordered_map>
#include "engine_shared.h"
#include <DataStructures/GrowingArray.h>
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
	AssetsContainer() = default;
	~AssetsContainer();
	void Initiate();

	void Update();

	void ReloadTexture(Texture* texture);


	void AddTexture(Texture* pTexture, u64 key);

	Texture* GetTexture(u64 key);
	Sprite* GetSprite(u64 key);
	Effect* GetEffect(u64 key);
	Material* GetMaterial(u64 key);

	template<typename T>
	RefPointer<T> GetModel(u64 key);

	Model* GetModelDirect(u64 key);

	//u64 GetModelKey(const char* path) const;


	template<typename T>
	HashType LoadModel(std::string path, std::string effect_filepath, bool thread = true, int option = 0);

	u64 LoadTexture(std::string path, bool make_mips = false);
	u64 LoadEffect(std::string path);
	u64 LoadSprite(std::string path);
	u64 LoadMaterial(std::string path);


	CompiledShader* GetShader(const char* key) const;

	ShaderFactory* GetShaderFactory();
private:
#ifndef FINAL
	FileWatcher* m_Watcher = nullptr;
#endif


	std::map<u64, Texture*> m_Textures;
	std::map<u64, Effect*> m_Effects;
	std::map<HashType, RefPointer<Model>> m_Models;
	std::map<u64, Sprite*> m_Sprites;
	std::map<u64, Material*> m_Materials;




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
RefPointer<T> AssetsContainer::GetModel(u64 key)
{
	auto it = m_Models.find(key);

	if (it != m_Models.end())
		return it->second;

}