#pragma once
#include "snowblind_shared.h"
class Level;

namespace Snowblind
{
	class Engine;
};
	class EntityManager;

namespace Cache
{
	class LevelManager
	{
	public:
		LevelManager() = default;
		void Initiate();
		void CleanUp();
		Level* LoadLevel(const std::string& level_filepath);
		void SaveLevel();
	private:
		
		void CreateEntitiy(const std::string& entity_filepath, JSONReader& level_reader, JSONElement::ConstMemberIterator it);
		void CreateTerrain(const std::string& terrain_path);

		void CreatePhysicsComponent();
		void CreateGraphicsComponent();
		void CreateInputComponent();
		void CreateAIComponent();
		void CreateNetworkComponent();



		Level* m_CurrentLevel = nullptr;
		Snowblind::Engine* m_Engine = nullptr;
		EntityManager* m_EntityManager = nullptr;
	};
};