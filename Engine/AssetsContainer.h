#pragma once
#include <unordered_map>

class FBXFactory;
class CModelImporter;

namespace Snowblind
{
	class CShaderFactory;
	class CModel;
	class CEffect;
	class CTexture;
	class CSprite;
	class CAssetsContainer
	{
	public:
		CAssetsContainer();
		~CAssetsContainer();

		void Initiate();

		void Update();

		CTexture* GetTexture(const std::string& aFilePath);
		CEffect* GetEffect(const std::string& aFilePath);
		CModel* GetModel(const std::string& aFilePath);
		const std::string& LoadModel(const std::string& aFilePath, const std::string& effect);

	private:
		
		std::unordered_map<std::string, CTexture*> myTextures;
		std::unordered_map<std::string, CEffect*> myEffects;
		std::unordered_map<std::string, CModel*> myModels;
		std::unordered_map<std::string, CSprite*> mySprites;

		
		void LoadTexture(const std::string& aFilePath);
		void LoadEffect(const std::string& aFilePath);

		CShaderFactory* myShaderFactory;
		CModelImporter* myModelLoader;

	};
};