#pragma once
#include <unordered_map>

namespace CommonLib
{
	class FileWatcher;
};


namespace Snowblind
{
	class CEffect;
	class CEffectContainer
	{
	public:

		static CEffectContainer* GetInstance();
		static void Destroy();
		CEffect* GetEffect(const std::string& aFileName);
		void Update();

	private:
		static CEffectContainer* myInstance;
		CEffectContainer();
		~CEffectContainer();

		CommonLib::FileWatcher* myFileWatcher;
		void Reload(const std::string& aFilePath);
		void LoadEffect(const std::string& aFileName);
		std::unordered_map<std::string, CEffect*>myEffects;
	};
};