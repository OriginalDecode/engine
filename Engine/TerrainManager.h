#pragma once
#include <map>
#include "engine_shared.h"

class Terrain;
class TerrainManager
{
public:
	TerrainManager() = default;
	bool CleanUp();
	Terrain* GetTerrain(u64 terrain_hash);
	Terrain* GetTerrain(const std::string& terrain_name);

	void AddTerrain(u64 hash, Terrain* terrain);

private:
	bool m_FinishedLoading = false;
	std::map<u64, Terrain*> m_Terrain;
	void LoadTerrain(u64 terrain_hash);
	void LoadTerrain(const std::string& terrain_name);
};
