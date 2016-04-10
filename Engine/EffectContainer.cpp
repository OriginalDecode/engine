#include "stdafx.h"
#include "EffectContainer.h"
#include "EngineDefines.h"
#include "Effect.h"
namespace Snowblind
{
	CEffectContainer* CEffectContainer::myInstance = nullptr;

	CEffectContainer::CEffectContainer()
	{
	}

	CEffectContainer::~CEffectContainer()
	{
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
			return LoadEffect(aFileName);

		return myEffects[aFileName];
	}



	CEffect* CEffectContainer::LoadEffect(const std::string& aFileName)
	{
		CEffect* tempEffect = nullptr;
		tempEffect = new CEffect();
		tempEffect->Initiate(aFileName);


		myEffects.insert(std::pair<std::string, CEffect*>(aFileName, tempEffect));
		return tempEffect;
	}

}