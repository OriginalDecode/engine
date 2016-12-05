#include "stdafx.h"
#include "LevelFactory.h"

#include "Engine.h"
#include <EntityManager.h>
#include "Terrain.h"

void LevelFactory::Initiate()
{
	m_Engine = Snowblind::Engine::GetInstance();
	m_EntityManager = m_Engine->GetEntityManager();
}

bool LevelFactory::CreateLevel(const std::string& level_path)
{
	//m_Engine->GetEntityManager()->CleanUp();
	//m_Engine->GetEntityManager()->Initiate();

	JSONReader read_level(level_path);
	const JSONElement& el = read_level.GetElement("root");


	CreateTerrain("Data/Textures/flat_height.tga");
	for (JSONElement::ConstMemberIterator it = el.MemberBegin(); it != el.MemberEnd(); it++)
	{
		CreateEntitiy(it->value["entity"].GetString());
	}

	return true;
}

void LevelFactory::CreateEntitiy(const std::string& entity_filepath, JSONReader& level_reader, JSONElement::ConstMemberIterator it)
{
	JSONReader entityReader(entity_filepath);
	Entity e = m_EntityManager->CreateEntity();
}

void LevelFactory::CreateTerrain(const std::string& terrain_path)
{
	Snowblind::Engine::GetInstance()->GetThreadpool().AddWork(
		Work([&]() {
		Snowblind::CTerrain* terrain = m_Engine->CreateTerrain(terrain_path, CU::Vector3f(0, 0, 0), CU::Vector2f(512, 512));
		terrain->AddNormalMap("Data/Textures/normal.dds");
		//myTerrain.Add(terrain);

		//terrain = myEngine->CreateTerrain("Data/Textures/t_1.tga", CU::Vector3f(0, 0, 510), CU::Vector2f(512, 512));
		//terrain->AddNormalMap("Data/Textures/t1_n.dds");
		//myTerrain.Add(terrain);

		//terrain = myEngine->CreateTerrain("Data/Textures/t_2.tga", CU::Vector3f(510, 0, 0), CU::Vector2f(512, 512));
		//terrain->AddNormalMap("Data/Textures/t2_n.dds");
		//myTerrain.Add(terrain);

		//terrain = myEngine->CreateTerrain("Data/Textures/t_3.tga", CU::Vector3f(510, 0, 510), CU::Vector2f(512, 512));
		//terrain->AddNormalMap("Data/Textures/t3_n.dds");
		//myTerrain.Add(terrain);
		/*
		for (s32 i = 0; i < myTerrain.Size(); i++)
		{
		myTerrainBodies.Add(myPhysicsManager->CreateBody());
		myPhysicsManager->Add(myTerrainBodies[i]->InitAsTerrain(myTerrain[i]->GetVerticeArrayCopy(), myTerrain[i]->GetIndexArrayCopy()));
		}*/

	}));
}
