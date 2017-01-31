#pragma once
#include <map>
#include <unordered_map>

class CModelImporter;
class FileWatcher;
namespace Hex
{
	class ShaderFactory;
	class CModel;
	class Effect;
	class Texture;
	class Sprite;
	struct CompiledShader;
};

class CAssetsContainer
{
public:
	~CAssetsContainer();

	void Initiate();

	void Update();

	void ReloadTexture(Hex::Texture* texture);
	Hex::Texture* GetTexture(const std::string& aFilePath);

	Hex::Effect* GetEffect(const std::string& aFilePath);
	Hex::CModel* GetModel(const std::string& aFilePath);
	const std::string& LoadModel(const std::string& aFilePath, const std::string& effect);

private:
	FileWatcher* m_TextureWatcher = nullptr;

	std::unordered_map<std::string, Hex::Texture*> myTextures;
	std::unordered_map<std::string, Hex::Effect*> myEffects;
	std::unordered_map<std::string, Hex::CModel*> myModels;
	std::unordered_map<std::string, Hex::Sprite*> mySprites;

	bool LoadTexture(const std::string& aFilePath);
	void LoadEffect(const std::string& aFilePath);

	Hex::ShaderFactory* myShaderFactory;
	CModelImporter* myModelLoader;
};
