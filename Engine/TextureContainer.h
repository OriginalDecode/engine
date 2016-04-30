#pragma once

namespace Snowblind
{
	class CTexture;

	class CTextureContainer
	{
	public:
		
		
		static CTextureContainer* GetInstance();
		static void Destroy();
		CTexture* GetTexture(const std::string& aTexturePath);
		

	private:
		static CTextureContainer* myInstance;
		CTextureContainer();
		~CTextureContainer();
		
		CTexture* LoadTexture(const std::string& aTexturePath);


		std::unordered_map<std::string, CTexture*> myTextures;


	};
};