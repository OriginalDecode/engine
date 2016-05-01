#include "stdafx.h"
#include "EffectContainer.h"
#include <functional>
#include "FileWatcher.h"
namespace Snowblind
{
	CEffectContainer* CEffectContainer::myInstance = nullptr;

	CEffectContainer::CEffectContainer()
	{
		myFileWatcher = new CommonLib::FileWatcher();
	}

	CEffectContainer::~CEffectContainer()
	{
		SAFE_DELETE(myFileWatcher);
		for (auto it = myEffects.begin(); it != myEffects.end(); ++it)
		{
			SAFE_DELETE(it->second);
		}
	}

	CEffectContainer* CEffectContainer::GetInstance()
	{
		if (myInstance == nullptr)
		{
			myInstance = new CEffectContainer();
		}
		return myInstance;
	}

	void CEffectContainer::Destroy()
	{
		if (myInstance != nullptr)
		{
			SAFE_DELETE(myInstance);
		}
	}

	CEffect* CEffectContainer::GetEffect(const std::string& aFileName)
	{
		if (myEffects.find(aFileName) == myEffects.end())
		{
			LoadEffect(aFileName);

			myFileWatcher->WatchFileChangeWithDependencies(aFileName, std::bind(&CEffectContainer::Reload, this, std::placeholders::_1));
		}

		return myEffects[aFileName];
	}

	void CEffectContainer::Update()
	{
		myFileWatcher->FlushChanges();
	}

	void CEffectContainer::Reload(const std::string& aFilePath)
	{
		if (myEffects.find(aFilePath) != myEffects.end())
		{
			myEffects[aFilePath]->Initiate(aFilePath);
		}
	}

	void CEffectContainer::LoadEffect(const std::string& aFileName)
	{
		CEffect* tempEffect = nullptr;
		tempEffect = new CEffect();
		tempEffect->Initiate(aFileName);


		myEffects.insert(std::pair<std::string, CEffect*>(aFileName, tempEffect));
	}

}