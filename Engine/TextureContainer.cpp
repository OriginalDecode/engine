#include "stdafx.h"

namespace Snowblind
{
	CTextureContainer* CTextureContainer::myInstance = nullptr;
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
		if (myTextures.find(aTexturePath) == myTextures.end())
		{
			return LoadTexture(aTexturePath);
		}

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

		if (tempTexture->LoadTexture(aTexturePath.c_str()) == false)
		{
			delete tempTexture;
			tempTexture = nullptr;
		}
		myTextures[aTexturePath] = tempTexture;
		return myTextures[aTexturePath];
	}

};