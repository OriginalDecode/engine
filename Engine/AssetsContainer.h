#pragma once
#include <unordered_map>

class CModelImporter;
class FileWatcher;
namespace Snowblind
{
	class CShaderFactory;
	class CModel;
	class Effect;
	class Texture;
	class CSprite;
};

namespace Cache
{
	class CAssetsContainer
	{
	public:
		CAssetsContainer() = default;
		~CAssetsContainer();

		void Initiate();

		void Update();

		void ReloadTexture(Snowblind::Texture* texture);
		Snowblind::Texture* GetTexture(const std::string& aFilePath);

		Snowblind::Effect* GetEffect(const std::string& aFilePath);
		Snowblind::CModel* GetModel(const std::string& aFilePath);
		const std::string& LoadModel(const std::string& aFilePath, const std::string& effect);
		
	private:
		FileWatcher* m_TextureWatcher = nullptr;
		
		std::unordered_map<std::string, Snowblind::Texture*> myTextures;
		std::unordered_map<std::string, Snowblind::Effect*> myEffects;
		std::unordered_map<std::string, Snowblind::CModel*> myModels;
		std::unordered_map<std::string, Snowblind::CSprite*> mySprites;
		
		bool LoadTexture(const std::string& aFilePath);
		void LoadEffect(const std::string& aFilePath);

		Snowblind::CShaderFactory* myShaderFactory;
		CModelImporter* myModelLoader;
	};
};