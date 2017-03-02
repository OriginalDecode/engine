#pragma once
#include <map>
#include <unordered_map>

class CModelImporter;
class FileWatcher;
class ShaderFactory;
class CModel;
class Effect;
class Texture;
class Sprite;
struct CompiledShader;

class CAssetsContainer
{
public:
	~CAssetsContainer();

	void Initiate();

	void Update();

	void ReloadTexture(Texture* texture);
	Texture* GetTexture(const std::string& aFilePath);

	Effect* GetEffect(const std::string& aFilePath);
	CModel* GetModel(const std::string& aFilePath);
	const std::string& LoadModel(const std::string& aFilePath, const std::string& effect);

private:
	FileWatcher* m_TextureWatcher = nullptr;

	std::unordered_map<std::string, Texture*> myTextures;
	std::unordered_map<std::string, Effect*> myEffects;
	std::unordered_map<std::string, CModel*> myModels;
	std::unordered_map<std::string, Sprite*> mySprites;

	bool LoadTexture(const std::string& aFilePath);
	void LoadEffect(const std::string& aFilePath);

	ShaderFactory* myShaderFactory;
	CModelImporter* myModelLoader;
};
