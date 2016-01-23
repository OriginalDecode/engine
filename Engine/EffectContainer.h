#pragma once
#include <unordered_map>
namespace Snowblind
{
	class CEffect;
	class CEffectContainer
	{
	public:

		static CEffectContainer* GetInstance();
		static void Destroy();
		CEffect* GetEffect(const std::string& aFileName);


	private:
		static CEffectContainer* myInstance;
		CEffectContainer();
		~CEffectContainer();

		CEffect* LoadEffect(const std::string& aFileName);
		std::unordered_map<std::string, CEffect*>myEffects;

	};
};