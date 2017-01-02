#include "stdafx.h"
#include "TerrainManager.h"
#include "Engine.h"
#include <DL_Debug.h>
#include "../CommonLib/DataStructures/Hashmap/Hash.h"
#include "Terrain.h"
namespace Cache
{
	bool TerrainManager::CleanUp()
	{
		for (auto it = m_Terrain.begin(); it != m_Terrain.end(); it++)
		{
			it->second->CleanUp();
			delete it->second;
			it->second = nullptr;
		}
		return true;
	}

	Hex::CTerrain* TerrainManager::GetTerrain(const std::string& terrain_name)
	{
		LoadTerrain(terrain_name);
		return GetTerrain(Hash(terrain_name.c_str()));
	}

	void TerrainManager::LoadTerrain(const std::string& terrain_name)
	{
		if (m_Terrain.find(Hash(terrain_name.c_str())) == m_Terrain.end())
		{
			Hex::CTerrain* terrain = new Hex::CTerrain;
			m_Terrain[Hash(terrain_name.c_str())] = terrain;
			DL_MESSAGE("Failed to find terrain in container, creating new terrain object!");
		}
	}

	Hex::CTerrain* TerrainManager::GetTerrain(u32 terrain_hash)
	{
		if (m_Terrain.find(terrain_hash) == m_Terrain.end())
		{
			LoadTerrain(terrain_hash);
		}
		return m_Terrain[terrain_hash];
	}

	void TerrainManager::LoadTerrain(u32 terrain_hash)
	{
		Hex::CTerrain* terrain = new Hex::CTerrain;
		m_Terrain[terrain_hash] = terrain;
	}
};