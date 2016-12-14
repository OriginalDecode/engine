#pragma once

#include <JSON/JSONReader.h>
#include "snowblind_shared.h"


class EntityManager;
class PhysicsManager;

class LevelFactory
{
public:
	LevelFactory() = default;

	void Initiate();
	bool CreateLevel(const std::string& level_path);

private:

	void CreateEntitiy(const std::string& entity_filepath, JSONElement::ConstMemberIterator it);
	void CreateTerrain(std::string terrain_path);

	void CreateTranslationComponent(Entity entity_id, const CU::Vector3f& position);
	void CreatePhysicsComponent(JSONReader& entity_reader, Entity entity_id);
	void CreateCameraComponent(JSONReader& entity_reader, Entity entity_id);

	void CreateGraphicsComponent(JSONReader& entity_reader, Entity entity_id, JSONElement::ConstMemberIterator it);
	void CreateLightComponent(JSONReader& entity_reader, Entity entity_id, JSONElement::ConstMemberIterator it);

	void CreateInputComponent(JSONReader& entity_reader, Entity entity_id);
	void CreateAIComponent(JSONReader& entity_reader, Entity entity_id);
	void CreateNetworkComponent(JSONReader& entity_reader, Entity entity_id);
	JSONReader m_LevelReader;

	Snowblind::Engine* m_Engine			= nullptr;
	EntityManager* m_EntityManager		= nullptr;
	PhysicsManager* m_PhysicsManager	= nullptr;


};
