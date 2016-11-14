#pragma once
#include <map>
#include "snowblind_shared.h"
namespace Snowblind
{
	class CTerrain;
};

namespace Cache
{
	class TerrainManager
	{
	public:
		TerrainManager() = default;
		bool CleanUp();
		Snowblind::CTerrain* GetTerrain(u32 terrain_hash);
		Snowblind::CTerrain* GetTerrain(const std::string& terrain_name);
	private:
		bool m_FinishedLoading = false;
		std::map<u32, Snowblind::CTerrain*> m_Terrain;
		void LoadTerrain(u32 terrain_hash);
		void LoadTerrain(const std::string& terrain_name);
	};
};