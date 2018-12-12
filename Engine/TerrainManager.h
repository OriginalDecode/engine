#pragma once
#include <map>
#include "engine_shared.h"

class Terrain;
class TerrainManager
{
public:
	TerrainManager() = default;
	bool CleanUp();
	Terrain* GetTerrain(uint64 terrain_hash);
	Terrain* GetTerrain(const std::string& terrain_name);

	void AddTerrain(uint64 hash, Terrain* terrain);

private:
	bool m_FinishedLoading = false;
	std::map<uint64, Terrain*> m_Terrain;
	void LoadTerrain(uint64 terrain_hash);
	void LoadTerrain(const std::string& terrain_name);
};
