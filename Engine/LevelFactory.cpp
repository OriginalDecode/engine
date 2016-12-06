#include "stdafx.h"
#include "LevelFactory.h"

#include "Engine.h"
#include <EntityManager.h>
#include "Terrain.h"

#include <PhysicsComponent.h>
#include <RenderComponent.h>
#include <TranslationComponent.h>
#include <LightComponent.h>
#include <InputComponent.h>
#include <AIComponent.h>
#include <NetworkComponent.h>
#include <CameraComponent.h>

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
		CreateEntitiy(it->value["entity"].GetString(), read_level, it);
	}

	return true;
}

void LevelFactory::CreateEntitiy(const std::string& entity_filepath, JSONReader& level_reader, JSONElement::ConstMemberIterator it)
{
	JSONReader entity_reader(entity_filepath);
	Entity e = m_EntityManager->CreateEntity();


	CreateTranslationComponent(e);

	if (entity_reader.HasElement("graphics"))
		CreateGraphicsComponent(entity_reader, e);

	if (entity_reader.HasElement("physics"))
		CreatePhysicsComponent(entity_reader, e);

	if (entity_reader.HasElement("camera"))
		CreateCameraComponent(entity_reader, e);

	if (entity_reader.HasElement("controller"))
	{
		if(entity_reader.ReadElement("controller") == "input")
		CreateInputComponent(entity_reader, e);

		if (entity_reader.ReadElement("controller") == "network")
			CreateNetworkComponent(entity_reader, e);

		if (entity_reader.ReadElement("controller") == "ai")
			CreateAIComponent(entity_reader, e);
	}
}

void LevelFactory::CreateTranslationComponent(JSONReader& entity_reader, Entity entity_id)
{
	m_EntityManager->AddComponent<TranslationComponent>(e);
	TranslationComponent& component = m_EntityManager->GetComponent<TranslationComponent>(e);
	//handle start position?
}

void LevelFactory::CreateGraphicsComponent(JSONReader& entity_reader, Entity entity_id)
{
	m_EntityManager->AddComponent<RenderComponent>(entity_id);
	RenderComponent& component = m_EntityManager->GetComponent<RenderComponent>(entity_id);

	component.myModelID = m_Engine->LoadModel(
		entity_reader.ReadElement("model"),
		entity_reader.ReadElement("shader"));
}

void LevelFactory::CreatePhysicsComponent(JSONReader& entity_reader, Entity entity_id)
{
	m_EntityManager->AddComponent<PhysicsComponent>(entity_id);
	PhysicsComponent& component = m_EntityManager->GetComponent<PhysicsComponent>(entity_id);
	//create a body and stuff. Read what shape it should be etc...
}

void LevelFactory::CreateCameraComponent(JSONReader& entity_reader, Entity entity_id)
{
	m_EntityManager->AddComponent<CameraComponent>(entity_id);
	CameraComponent& component = m_EntityManager->GetComponent<CameraComponent>(entity_id);

	component.m_Camera = Snowblind::Engine::GetInstance()->GetCamera();
}

void LevelFactory::CreateInputComponent(JSONReader& entity_reader, Entity entity_id)
{
	m_EntityManager->AddComponent<InputComponent>(entity_id);
	InputComponent& component = m_EntityManager->GetComponent<InputComponent>(entity_id);
	
	component.m_ID = 0; //talk to network_manager for this id
	component.m_InputHandle = new InputHandle;
	component.m_InputHandle->Initiate(component.m_ID);

	//Read Input stuffies
	assert(false);
}

void LevelFactory::CreateAIComponent(JSONReader& entity_reader, Entity entity_id)
{
	m_EntityManager->AddComponent<AIComponent>(entity_id);
	AIComponent& component = m_EntityManager->GetComponent<AIComponent>(entity_id);

	assert(false);
}

void LevelFactory::CreateNetworkComponent(JSONReader& entity_reader, Entity entity_id)
{
	m_EntityManager->AddComponent<NetworkComponent>(entity_id);
	NetworkComponent& component = m_EntityManager->GetComponent<NetworkComponent>(entity_id);

	assert(false);
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
