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
	Texture* GetTexture(const cl::HashString& path);

	Sprite* GetSprite(const cl::HashString& path);
	Effect* GetEffect(const cl::HashString& path);
	Model* GetModel(const cl::HashString& path);
	cl::HashString LoadModel(const cl::HashString& path, std::string effect, bool thread = true);
	template<typename T>
	cl::HashString LoadModel(const cl::HashString& path, std::string effect_filepath, T* pModel, bool thread = true);


private:
#ifndef FINAL
	FileWatcher* m_Watcher = nullptr;
#endif


	Ticket_Mutex m_Mutex;
	std::map<cl::HashString, Texture*> m_Textures;
	std::map<cl::HashString, Effect*> m_Effects;
	std::map<cl::HashString, Model*> m_Models;
	std::map<cl::HashString, Sprite*> m_Sprites;

	bool LoadTexture(const cl::HashString& filepath);
	Effect* LoadEffect(const cl::HashString& filepath);

	Sprite* LoadSprite(const cl::HashString& path);

	ShaderFactory* m_ShaderFactory;
	CModelImporter* m_ModelLoader;

};

template<typename T>
cl::HashString AssetsContainer::LoadModel(const cl::HashString& path, std::string effect_filepath, T* pModel, bool thread /*= true*/)
{
	if (m_Models.find(path) != m_Models.end())
		return path;
	DL_MESSAGE("Loading model : %s", path.c_str());
	T* model = new T;
	m_Models.emplace(path, model);

	if (thread)
	{
		Engine::GetInstance()->GetThreadpool().AddWork(Work([=]() {
			m_ModelLoader->LoadModel(model, path.c_str(), effect_filepath);
			model->Initiate(path.c_str());
		}));
	}
	else
	{
		m_ModelLoader->LoadModel(model, path.c_str(), effect_filepath);
		model->Initiate(path.c_str());
	}

	return path;
}
