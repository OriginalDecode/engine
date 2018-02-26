#include "stdafx.h"
#include "TerrainManager.h"
#include "Engine.h"
#include <DL_Debug.h>
#include "../CommonLib/DataStructures/Hashmap/Hash.h"
#include "Terrain.h"

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

Terrain* TerrainManager::GetTerrain(const std::string& terrain_name)
{
	LoadTerrain(terrain_name);
	return GetTerrain(Hash(terrain_name.c_str()));
}

void TerrainManager::AddTerrain(u64 hash, Terrain* terrain)
{
	auto it = m_Terrain.find(hash);
	if (it != m_Terrain.end())
		return;

	m_Terrain.insert(std::make_pair(hash, terrain));

}


void TerrainManager::LoadTerrain(const std::string& terrain_name)
{
	if (m_Terrain.find(Hash(terrain_name.c_str())) == m_Terrain.end())
	{
		Terrain* terrain = new Terrain;
		m_Terrain[Hash(terrain_name.c_str())] = terrain;
		//DL_MESSAGE("Failed to find terrain in container, creating new terrain object!");
	}
}

Terrain* TerrainManager::GetTerrain(u64 terrain_hash)
{
	if (m_Terrain.find(terrain_hash) == m_Terrain.end())
		return nullptr;

	return m_Terrain[terrain_hash];
}

void TerrainManager::LoadTerrain(u64 terrain_hash)
{
	Terrain* terrain = new Terrain;
	m_Terrain[terrain_hash] = terrain;
}
