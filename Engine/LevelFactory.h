#pragma once

#include <JSON/JSONReader.h>
#include "snowblind_shared.h"
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
	TreeDweller* CreateEntitiy(const std::string& entity_filepath, const CU::Vector3f& position);

private:

	void CreateEntitiy(const std::string& entity_filepath, JSONElement::ConstMemberIterator it);
	void CreateTerrain(std::string terrain_path);

	void CreateTranslationComponent(Entity entity_id, const CU::Vector3f& position);
	void CreatePhysicsComponent(JSONReader& entity_reader, Entity entity_id);
	void CreateCameraComponent(JSONReader& entity_reader, Entity entity_id);

	void CreateGraphicsComponent(JSONReader& entity_reader, Entity entity_id, JSONElement::ConstMemberIterator it);
	void CreateGraphicsComponent(JSONReader& entity_reader, Entity entity_id);

	void CreateLightComponent(JSONReader& entity_reader, Entity entity_id, JSONElement::ConstMemberIterator it);

	void CreateInputComponent(JSONReader& entity_reader, Entity entity_id);
	void CreateAIComponent(JSONReader& entity_reader, Entity entity_id);
	void CreateNetworkComponent(JSONReader& entity_reader, Entity entity_id);

	CU::GrowingArray<TreeDweller*> m_DwellerList;

	JSONReader m_LevelReader;

	Hex::Engine* m_Engine			= nullptr;
	EntityManager* m_EntityManager;
	PhysicsManager* m_PhysicsManager	= nullptr;
	bool sponza = false;

	void CreateDebugComponent(Entity e, bool isLight, s32 flags);
};
