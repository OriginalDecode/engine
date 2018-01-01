#pragma once

#include <JSON/JSONReader.h>
#include "engine_shared.h"
#include <EntityManager.h>

class EntityManager;
class PhysicsManager;
class TreeDweller;
class LevelFactory
{
public:
	LevelFactory() = default;

	void Initiate();
	bool CreateLevel(const std::string& level_path);
	CU::GrowingArray<TreeDweller*> GetDwellers() { return m_DwellerList; }
	
	void CreatePBLLevel(s32 steps);


	static void SaveLevel(std::string folder, std::string filename);

	static void CreateEntity(Entity e, EntityManager& em);
private:
	void CreateEntity(const std::string& entity_filepath);
	void CreateTerrain(std::string terrain_path);





	CU::GrowingArray<TreeDweller*> m_DwellerList;

	JSONReader m_LevelReader;

	Engine* m_Engine			= nullptr;
	EntityManager* m_EntityManager;
	PhysicsManager* m_PhysicsManager	= nullptr;
	bool sponza = false;

	void CreateDebugComponent(Entity e, bool isLight, s32 flags);
};
