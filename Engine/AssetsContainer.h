#pragma once
#include <unordered_map>

class FBXFactory;

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

		static void Create();
		static void Destroy();
		static CAssetsContainer* GetInstance();

		CTexture* GetTexture(const std::string& aFilePath);
		CEffect* GetEffect(const std::string& aFilePath);
		CModel* GetModel(const std::string& aFilePath, const std::string& effect);
		void Update();

	private:
		CAssetsContainer();
		~CAssetsContainer();
		static CAssetsContainer* myInstance;

		std::unordered_map<std::string, CTexture*> myTextures;
		std::unordered_map<std::string, CEffect*> myEffects;
		std::unordered_map<std::string, CModel*> myModels;
		std::unordered_map<std::string, CSprite*> mySprites;

		void LoadTexture(const std::string& aFilePath);
		void LoadEffect(const std::string& aFilePath);
		void LoadModel(const std::string& aFilePath, const std::string& effect);

		FBXFactory* myFactory;
		CShaderFactory* myShaderFactory;

	};
};