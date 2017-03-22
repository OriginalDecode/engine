#pragma once
#include <map>
#include <unordered_map>
#include "snowblind_shared.h"
class CModelImporter;
class FileWatcher;
class ShaderFactory;
class CModel;
class Effect;
class Texture;
class Sprite;
struct CompiledShader;
class Engine;
class AssetsContainer
{
public:
	~AssetsContainer();

	void Initiate();

	void Update();

	void ReloadTexture(Texture* texture);
	Texture* GetTexture(const std::string& aFilePath);

	Effect* GetEffect(const std::string& aFilePath);
	CModel* GetModel(const std::string& aFilePath);
	std::string LoadModel(std::string aFilePath, std::string effect);

private:
	Engine* m_Engine = nullptr;
	FileWatcher* m_TextureWatcher = nullptr;
	Ticket_Mutex m_Mutex;
	std::unordered_map<std::string, Texture*> myTextures;
	std::unordered_map<std::string, Effect*> myEffects;
	std::unordered_map<std::string, CModel*> myModels;
	std::unordered_map<std::string, Sprite*> mySprites;

	bool LoadTexture(const std::string& aFilePath);
	void LoadEffect(const std::string& aFilePath);

	ShaderFactory* myShaderFactory;
	CModelImporter* myModelLoader;
};
