#pragma once
#include <map>
#include "snowblind_shared.h"

class CTerrain;
class TerrainManager
{
public:
	TerrainManager() = default;
	bool CleanUp();
	CTerrain* GetTerrain(u32 terrain_hash);
	CTerrain* GetTerrain(const std::string& terrain_name);
private:
	bool m_FinishedLoading = false;
	std::map<u32, CTerrain*> m_Terrain;
	void LoadTerrain(u32 terrain_hash);
	void LoadTerrain(const std::string& terrain_name);
};
