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

	//u64 GetModelKey(const char* path) const;


	template<typename T>
	u64 LoadModel(std::string path, std::string effect_filepath, bool thread = true);

	u64 LoadTexture(std::string path, bool make_mips = false);
	u64 LoadEffect(std::string path);
	u64 LoadSprite(std::string path);
	u64 LoadMaterial(std::string path);


	void* GetShader(const char* key);
	void* GetShader(u64 key);

	void InsertShader(u64 key, void* shader);

private:
#ifndef FINAL
	FileWatcher* m_Watcher = nullptr;
#endif


	std::map<u64, Texture*> m_Textures;
	std::map<u64, Effect*> m_Effects;
	std::map<u64, RefPointer<Model>> m_Models;
	std::map<u64, Sprite*> m_Sprites;
	std::map<u64, Material*> m_Materials;

	std::map<u64, void*> m_Shaders;



	ShaderFactory* m_ShaderFactory;

};

template<typename T>
u64 AssetsContainer::LoadModel(std::string path, std::string effect_filepath, bool thread /*= true*/)
{
	if (!cl::file_exist(path) && path.find("default") != 0)
		DL_ASSERT("Failed to find the file!");

	
	u64 hash = Hash(path.c_str());
	Engine* engine = Engine::GetInstance();
	if (path.find("default") != path.npos)
		return g_DefaultModel;


	if (m_Models.find(hash) != m_Models.end())
		return hash;

	Effect* effect = engine->GetEffect(effect_filepath.c_str());

	m_Models.emplace(hash, RefPointer<Model>(new T));
	T* model = static_cast<T*>(m_Models.at(hash).GetData());
	model->SetKey(hash);
	if (thread)
	{
		engine->GetThreadpool().AddWork(Work([=]() {
			CModelImporter importer;
			importer.LoadModel<T>(path, model, effect);
			model->Initiate(path);
		}));
	}
	else
	{
		CModelImporter importer;
		importer.LoadModel<T>(path, model, effect);
		model->Initiate(path);
	}

	return hash;
}

template<typename T>
RefPointer<T> AssetsContainer::GetModel(u64 key)
{
	auto it = m_Models.find(key);

	if (it != m_Models.end())
		return it->second;

}