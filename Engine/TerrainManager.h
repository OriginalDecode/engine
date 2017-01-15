#pragma once
#include <map>
#include "snowblind_shared.h"
namespace Hex
{
	class CTerrain;
};

class TerrainManager
{
public:
	TerrainManager() = default;
	bool CleanUp();
	Hex::CTerrain* GetTerrain(u32 terrain_hash);
	Hex::CTerrain* GetTerrain(const std::string& terrain_name);
private:
	bool m_FinishedLoading = false;
	std::map<u32, Hex::CTerrain*> m_Terrain;
	void LoadTerrain(u32 terrain_hash);
	void LoadTerrain(const std::string& terrain_name);
};
