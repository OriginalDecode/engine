#include "stdafx.h"
#include "LevelFactory.h"

#include "Engine.h"
#include <EntityManager.h>
#include <PhysicsManager.h>
#include "Terrain.h"

#include <PhysicsComponent.h>
#include <RenderComponent.h>
#include <TranslationComponent.h>
#include <DebugComponent.h>
#include <LightComponent.h>
#include <InputComponent.h>
#include <AIComponent.h>
#include <NetworkComponent.h>
#include <CameraComponent.h>
#include <RigidBody.h>

#include "../hashlist.h"

void LevelFactory::Initiate()
{
	m_Engine = Hex::Engine::GetInstance();
	m_EntityManager = m_Engine->GetEntityManager();
	m_PhysicsManager = m_Engine->GetPhysicsManager();
}

bool LevelFactory::CreateLevel(const std::string& level_path)
{
	//m_Engine->GetEntityManager()->CleanUp();
	//m_Engine->GetEntityManager()->Initiate();
	m_LevelReader.OpenDocument(level_path);

	const JSONElement& el = m_LevelReader.GetElement("root");


	m_Engine->GetThreadpool().AddWork(Work([&]() {CreateTerrain("Data/Textures/t_1.tga"); }));
	for (JSONElement::ConstMemberIterator it = el.MemberBegin(); it != el.MemberEnd(); it++)
	{
		CreateEntitiy(it->value["entity"].GetString(), it);
	}

	return true;
}

void LevelFactory::CreateEntitiy(const std::string& entity_filepath, JSONElement::ConstMemberIterator it)
{
	JSONReader entity_reader(entity_filepath);
	Entity e = m_EntityManager->CreateEntity();

	CU::Vector3f pos;
	m_LevelReader._ReadElement(it->value["position"], pos);
	CreateTranslationComponent(e, pos);

	if (entity_reader.HasElement("graphics"))
		CreateGraphicsComponent(entity_reader, e, it);

	if (entity_reader.HasElement("physics"))
		CreatePhysicsComponent(entity_reader, e);

	if (entity_reader.HasElement("camera"))
		CreateCameraComponent(entity_reader, e);

	if (entity_reader.HasElement("light"))
		CreateLightComponent(entity_reader, e, it);


	if (entity_reader.HasElement("controller"))
	{
		if (entity_reader.ReadElement("controller") == "input")
			CreateInputComponent(entity_reader, e);
		else if (entity_reader.ReadElement("controller") == "network")
			CreateNetworkComponent(entity_reader, e);
		else if (entity_reader.ReadElement("controller") == "ai")
			CreateAIComponent(entity_reader, e);
		else
			DL_ASSERT("Failed to find correct input controller tag!");

		if (entity_reader.ReadElement("controller") != "input")
		{
			CreateEditingPhysicsComponent(e);
		}
	}
	else
	{
		if(!entity_reader.HasElement("light"))
			CreateEditingPhysicsComponent(e);
	}
}

void LevelFactory::CreateTranslationComponent(Entity entity_id, const CU::Vector3f& position)
{
	m_EntityManager->AddComponent<TranslationComponent>(entity_id);
	TranslationComponent& component = m_EntityManager->GetComponent<TranslationComponent>(entity_id);
	component.myOrientation.SetPosition(position);
}

void LevelFactory::CreateGraphicsComponent(JSONReader& entity_reader, Entity entity_id, JSONElement::ConstMemberIterator it)
{
	m_EntityManager->AddComponent<RenderComponent>(entity_id);
	RenderComponent& component = m_EntityManager->GetComponent<RenderComponent>(entity_id);

	const JSONElement& el = entity_reader.GetElement("graphics");
	component.myModelID = m_Engine->LoadModel(
		el["model"].GetString(),
		el["shader"].GetString());


	CU::Vector3f scale;
	m_LevelReader._ReadElement(it->value["scale"], scale);
	CU::Vector3f rotation;
	m_LevelReader._ReadElement(it->value["rotation"], rotation); 

	component.m_Rotation = rotation;

	component.scale = scale;
	component.scale.w = 1.f;
}

void LevelFactory::CreatePhysicsComponent(JSONReader& entity_reader, Entity entity_id)
{
	m_EntityManager->AddComponent<PhysicsComponent>(entity_id);
	PhysicsComponent& component = m_EntityManager->GetComponent<PhysicsComponent>(entity_id);

	const JSONElement& el = entity_reader.GetElement("physics");
	float object_mass = (float)el["mass"].GetDouble();

	component.myBody = m_PhysicsManager->CreateBody();
	btRigidBody* phys_body = nullptr;

	std::string shape = el["shape"].GetString();
	if (shape == "mesh")
	{
		phys_body = component.myBody->InitAsBox(1, 1, 1, { 0,0,0 });
	}
	else if (shape == "box")
	{
		phys_body = component.myBody->InitAsBox(1, 1, 1, { 0,0,0 });
	}
	else if (shape == "sphere")
	{
		phys_body = component.myBody->InitAsSphere(1, object_mass, m_PhysicsManager->GetGravityForce(), scientific_constants::pressure::air_pressure, { 0,0,0 });
	}

	m_PhysicsManager->Add(phys_body);
}

void LevelFactory::CreateEditingPhysicsComponent(Entity entity_id)
{
	RenderComponent& render = m_EntityManager->GetComponent<RenderComponent>(entity_id);
	CU::Vector3f whd = m_Engine->GetModel(render.myModelID)->GetWHD();


	m_EntityManager->AddComponent<DebugComponent>(entity_id);
	DebugComponent& component = m_EntityManager->GetComponent<DebugComponent>(entity_id);
	component.m_Body = m_PhysicsManager->CreateBody();

	btGhostObject * ghostObject = component.m_Body->InitAsGhostObject(whd.x, whd.y, whd.z, { 0, 0, 0 });

	m_PhysicsManager->Add(ghostObject);
}

void LevelFactory::CreateCameraComponent(JSONReader& /*entity_reader*/, Entity entity_id)
{
	m_EntityManager->AddComponent<CameraComponent>(entity_id);
	CameraComponent& component = m_EntityManager->GetComponent<CameraComponent>(entity_id);

	component.m_Camera = Hex::Engine::GetInstance()->GetCamera();
}

void LevelFactory::CreateLightComponent(JSONReader& entity_reader, Entity entity_id, JSONElement::ConstMemberIterator it)
{
	m_EntityManager->AddComponent<LightComponent>(entity_id);
	LightComponent& component = m_EntityManager->GetComponent<LightComponent>(entity_id);

	std::string type;
	entity_reader.ReadElement("light", "type", type);

	m_LevelReader._ReadElement(it->value["color"], component.color);
	m_LevelReader.ReadElement(it->value["range"], component.range);

	component.color.x /= 255;
	component.color.y /= 255;
	component.color.z /= 255;

	if (type == "pointlight")
	{
		component.myType = eLightType::ePOINTLIGHT;

	}
	else if (type == "spotlight")
	{
		component.myType = eLightType::eSPOTLIGHT;

		m_LevelReader._ReadElement(it->value["direction"], component.direction);

		component.orientation = CU::Matrix44f::CreateRotateAroundZ(CL::DegreeToRad(component.direction.z)) * component.orientation;
		component.orientation = CU::Matrix44f::CreateRotateAroundY(CL::DegreeToRad(component.direction.y)) * component.orientation;
		component.orientation = CU::Matrix44f::CreateRotateAroundX(CL::DegreeToRad(component.direction.x)) * component.orientation;

		m_LevelReader.ReadElement(it->value["angle"], component.angle);
		component.angle = CL::DegreeToRad(component.angle);

	}

}

void LevelFactory::CreateInputComponent(JSONReader& /*entity_reader*/, Entity entity_id)
{
	//Is this component needed?
	m_EntityManager->AddComponent<InputComponent>(entity_id);
	//InputComponent& component = m_EntityManager->GetComponent<InputComponent>(entity_id);
	//PhysicsComponent& component = m_EntityManager->GetComponent<PhysicsComponent>(entity_id);

	//component.m_InputHandle = new InputHandle; 
	//InputHandle handle = component.m_InputHandle;
	//handle->AddController(0);
	//handle->GetController(0)->GetState().m_ThumbLY


	//input.m_InputHandle->Bind(Hash("string_to_hash"), [&] { function(); }); <- this is how you bind
}

void LevelFactory::CreateAIComponent(JSONReader& /*entity_reader*/, Entity entity_id)
{
	m_EntityManager->AddComponent<AIComponent>(entity_id);
	//AIComponent& component = m_EntityManager->GetComponent<AIComponent>(entity_id);
	//read behaviour trees and stuff here...
}

void LevelFactory::CreateNetworkComponent(JSONReader& /*entity_reader*/, Entity entity_id)
{
	m_EntityManager->AddComponent<NetworkComponent>(entity_id);
	//NetworkComponent& component = m_EntityManager->GetComponent<NetworkComponent>(entity_id);

}

void LevelFactory::CreateTerrain(std::string terrain_path)
{
	Hex::CTerrain* terrain = Hex::Engine::GetInstance()->CreateTerrain(terrain_path, CU::Vector3f(0, 0, 0), CU::Vector2f(512, 512));
	terrain->AddNormalMap("Data/Textures/t1_n.dds");
	/*
	Work([&](std::string texture) {
		Hex::CTerrain* terrain = m_Engine->CreateTerrain(texture, CU::Vector3f(0, 0, 0), CU::Vector2f(512, 512));
		terrain->AddNormalMap("Data/Textures/normal.dds");
	});
	*/

	/*
	myTerrain.Add(terrain);

	terrain = myEngine->CreateTerrain("Data/Textures/t_1.tga", CU::Vector3f(0, 0, 510), CU::Vector2f(512, 512));
	terrain->AddNormalMap("Data/Textures/t1_n.dds");
	myTerrain.Add(terrain);

	terrain = myEngine->CreateTerrain("Data/Textures/t_2.tga", CU::Vector3f(510, 0, 0), CU::Vector2f(512, 512));
	terrain->AddNormalMap("Data/Textures/t2_n.dds");
	myTerrain.Add(terrain);

	terrain = myEngine->CreateTerrain("Data/Textures/t_3.tga", CU::Vector3f(510, 0, 510), CU::Vector2f(512, 512));
	terrain->AddNormalMap("Data/Textures/t3_n.dds");
	myTerrain.Add(terrain);

	for (s32 i = 0; i < myTerrain.Size(); i++)
	{
		myTerrainBodies.Add(myPhysicsManager->CreateBody());
		myPhysicsManager->Add(myTerrainBodies[i]->InitAsTerrain(myTerrain[i]->GetVerticeArrayCopy(), myTerrain[i]->GetIndexArrayCopy()));
	}


	}));*/
}


