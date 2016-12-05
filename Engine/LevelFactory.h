#pragma once
class LevelFactory
{
public:
	LevelFactory() = default;

	void Initiate();
	bool CreateLevel(const std::string& level_path);

private:

	void CreateEntitiy(const std::string& entity_filepath, JSONReader& level_reader, JSONElement::ConstMemberIterator it);
	void CreateTerrain(const std::string& terrain_path);

	void CreatePhysicsComponent();
	void CreateGraphicsComponent();
	void CreateInputComponent();
	void CreateAIComponent();
	void CreateNetworkComponent();

	Snowblind::Engine* m_Engine = nullptr;
	EntityManager* m_EntityManager = nullptr;

};

