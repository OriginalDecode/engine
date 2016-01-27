#include "TextureContainer.h"
#include "EngineDefines.h"
#include "Texture.h"

namespace Snowblind
{

	CTextureContainer* CTextureContainer::GetInstance()
	{
		if (myInstance == nullptr)
		{
			myInstance = new CTextureContainer();
		}
		return myInstance;
	}

	void CTextureContainer::Destroy()
	{
		if (myInstance != nullptr)
		{
			delete myInstance;
			myInstance = nullptr;
		}
	}

	CTexture* CTextureContainer::GetTexture(const std::string& aTexturePath)
	{
		return myTextures[aTexturePath];
	}

	CTextureContainer::CTextureContainer()
	{
	}


	CTextureContainer::~CTextureContainer()
	{
		for (auto it = myTextures.begin(); it != myTextures.end(); ++it)
		{
			SAFE_DELETE(it->second);
		}
	}

	CTexture* CTextureContainer::LoadTexture(const std::string& aTexturePath)
	{
		CTexture* tempTexture = new CTexture();


		return tempTexture;
	}

};