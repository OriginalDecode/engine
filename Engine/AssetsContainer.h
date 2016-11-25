#pragma once
#include <unordered_map>

class CModelImporter;
class FileWatcher;
namespace Snowblind
{
	class CShaderFactory;
	class CModel;
	class CEffect;
	class CTexture;
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

		void ReloadTexture(Snowblind::CTexture* texture);
		Snowblind::CTexture* GetTexture(const std::string& aFilePath);

		Snowblind::CEffect* GetEffect(const std::string& aFilePath);
		Snowblind::CModel* GetModel(const std::string& aFilePath);
		const std::string& LoadModel(const std::string& aFilePath, const std::string& effect);
		
	private:
		FileWatcher* m_TextureWatcher = nullptr;
		
		std::unordered_map<std::string, Snowblind::CTexture*> myTextures;
		std::unordered_map<std::string, Snowblind::CEffect*> myEffects;
		std::unordered_map<std::string, Snowblind::CModel*> myModels;
		std::unordered_map<std::string, Snowblind::CSprite*> mySprites;
		
		bool LoadTexture(const std::string& aFilePath);
		void LoadEffect(const std::string& aFilePath);

		Snowblind::CShaderFactory* myShaderFactory;
		CModelImporter* myModelLoader;
	};
};