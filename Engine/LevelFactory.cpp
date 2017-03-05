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
#include <GhostObject.h>
#include "../hashlist.h"
#include "TreeDweller.h"

void LevelFactory::Initiate()
{
	m_Engine = Engine::GetInstance();
	m_EntityManager = &m_Engine->GetEntityManager();
	m_PhysicsManager = m_Engine->GetPhysicsManager();
}

bool LevelFactory::CreateLevel(const std::string& level_path)
{
	//m_Engine->GetEntityManager()->CleanUp();
	//m_Engine->GetEntityManager()->Initiate();
	m_LevelReader.OpenDocument(level_path);

	const JSONElement& el = m_LevelReader.GetElement("root");


	m_Engine->GetThreadpool().AddWork(Work([&]() {CreateTerrain("Data/Textures/flat_height.tga"); }));
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

	s32 debug_flags = 0;

	m_DwellerList.Add(new TreeDweller);


	CU::Vector3f pos;
	m_LevelReader.ReadElement(it->value["position"], pos);
	CreateTranslationComponent(e, pos);
	bool hasLight = true;
	if (entity_reader.DocumentHasMember("graphics"))
	{
		CreateGraphicsComponent(entity_reader, e, it);
		hasLight = false;
		debug_flags |= EditObject::GRAPHICS;
	}

	if (entity_reader.DocumentHasMember("physics"))
	{
		CreatePhysicsComponent(entity_reader, e);
		debug_flags |= EditObject::PHYSICS;
	}

	if (entity_reader.DocumentHasMember("camera"))
	{
		CreateCameraComponent(entity_reader, e);
		//debug_flags |= EditObject::PHYSICS;
	}
	if (entity_reader.DocumentHasMember("light"))
	{
		CreateLightComponent(entity_reader, e, it);
		debug_flags |= EditObject::LIGHT;
	}

	if (entity_reader.DocumentHasMember("controller"))
	{
		if (entity_reader.ReadElement("controller") == "input")
		{
			CreateInputComponent(entity_reader, e);
			debug_flags |= EditObject::INPUT;
		}
		else if (entity_reader.ReadElement("controller") == "network")
		{
			CreateNetworkComponent(entity_reader, e);
			debug_flags |= EditObject::NETWORK;
		}
		else if (entity_reader.ReadElement("controller") == "ai")
		{
			CreateAIComponent(entity_reader, e);
			debug_flags |= EditObject::AI;
		}
		else
			DL_ASSERT("Failed to find correct input controller tag!");
	}

	CreateDebugComponent(e, hasLight, debug_flags);


	m_DwellerList.GetLast()->Initiate(e);

	TranslationComponent& translation = m_EntityManager->GetComponent<TranslationComponent>(e);

	/*if (sponza)
	{
		for (int i = 0; i < 80; i++)
		{
			Entity entity = m_EntityManager->CreateEntity();
			m_DwellerList.Add(new TreeDweller);
			CU::Vector3f random_position;

			CU::Vector3f position = translation.myOrientation.GetPosition();
			random_position.x = RANDOM(position.x - 128.f, position.x + 128.f);
			random_position.y = RANDOM(position.y, position.y + 64.f);
			random_position.z = RANDOM(position.z - 64.f, position.z + 64.f);
			CreateTranslationComponent(entity, random_position);

			m_EntityManager->AddComponent<LightComponent>(entity);
			LightComponent& light = m_EntityManager->GetComponent<LightComponent>(entity);
			m_DwellerList.GetLast()->AddComponent<LightComponent>(&light, TreeDweller::LIGHT);



			light.color.x = RANDOM(0.f, 1.f);
			light.color.y = RANDOM(0.f, 1.f);
			light.color.z = RANDOM(0.f, 1.f);

			light.myType = eLightType::ePOINTLIGHT;
			light.range = 25.f;
			m_DwellerList.GetLast()->Initiate(entity);
			CreateDebugComponent(entity, true, EditObject::LIGHT);

		}
	}*/

}

TreeDweller* LevelFactory::CreateEntitiy(const std::string& entity_filepath, const CU::Vector3f& position)
{
	Entity e = m_EntityManager->CreateEntity();
	JSONReader entity_reader(entity_filepath);
	s32 debug_flags = 0;

	TreeDweller* dweller = new TreeDweller;

	CreateTranslationComponent(e, position);

	if (entity_reader.DocumentHasMember("graphics"))
	{
		CreateGraphicsComponent(entity_reader, e);
		debug_flags |= EditObject::GRAPHICS;
	}

	if (entity_reader.DocumentHasMember("physics"))
	{
		CreatePhysicsComponent(entity_reader, e);
		debug_flags |= EditObject::PHYSICS;
	}

	dweller->Initiate(e);
	return dweller;
}

void LevelFactory::CreateTranslationComponent(Entity entity_id, const CU::Vector3f& position)
{
	m_EntityManager->AddComponent<TranslationComponent>(entity_id);

	TranslationComponent& component = m_EntityManager->GetComponent<TranslationComponent>(entity_id);
	m_DwellerList.GetLast()->AddComponent<TranslationComponent>(&component, TreeDweller::TRANSLATION);

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


	if (el["model"] == "Data/Model/sponza/Sponza_2.fbx")
		sponza = true;


	CU::Vector3f scale;
	m_LevelReader.ReadElement(it->value["scale"], scale);
	CU::Vector3f rotation;
	m_LevelReader.ReadElement(it->value["rotation"], rotation);

	component.m_Rotation = rotation;

	TranslationComponent& translation = m_EntityManager->GetComponent<TranslationComponent>(entity_id);
	translation.myOrientation = CU::Matrix44f::CreateRotateAroundY(CL::DegreeToRad(90.f) * rotation.y) * translation.myOrientation;
	translation.myOrientation = CU::Matrix44f::CreateRotateAroundZ(CL::DegreeToRad(90.f) * rotation.z) * translation.myOrientation;
	translation.myOrientation = CU::Matrix44f::CreateRotateAroundX(CL::DegreeToRad(90.f) * rotation.x) * translation.myOrientation;

	component.scale = scale;
	component.scale.w = 1.f;

	CU::Vector3f whd = m_Engine->GetModel(component.myModelID)->GetWHD();
	m_DwellerList.GetLast()->AddComponent<RenderComponent>(&component, TreeDweller::GRAPHICS);
	m_DwellerList.GetLast()->SetWHD(whd);
}

void LevelFactory::CreateGraphicsComponent(JSONReader& entity_reader, Entity entity_id)
{
	m_EntityManager->AddComponent<RenderComponent>(entity_id);

	RenderComponent& component = m_EntityManager->GetComponent<RenderComponent>(entity_id);
	const JSONElement& el = entity_reader.GetElement("graphics");
	component.myModelID = m_Engine->LoadModel(
		el["model"].GetString(),
		el["shader"].GetString());

	component.scale = CU::Vector4f(1,1,1,1);

	//CU::Vector3f whd = m_Engine->GetModel(component.myModelID)->GetWHD();
	//m_DwellerList.GetLast()->AddComponent<RenderComponent>(&component, TreeDweller::GRAPHICS);
	//m_DwellerList.GetLast()->SetWHD(whd);
}

void LevelFactory::CreatePhysicsComponent(JSONReader& entity_reader, Entity entity_id)
{
	m_EntityManager->AddComponent<PhysicsComponent>(entity_id);

	PhysicsComponent& component = m_EntityManager->GetComponent<PhysicsComponent>(entity_id);
	m_DwellerList.GetLast()->AddComponent<PhysicsComponent>(&component, TreeDweller::PHYSICS);

	const JSONElement& el = entity_reader.GetElement("physics");
	float object_mass = (float)el["mass"].GetDouble();

	component.myBody = m_PhysicsManager->CreateBody();
	btRigidBody* phys_body = nullptr;

	TranslationComponent& translation_component = m_EntityManager->GetComponent<TranslationComponent>(entity_id);
	


	std::string shape = el["shape"].GetString();
	if (shape == "mesh")
	{
		RenderComponent& render_component = m_EntityManager->GetComponent<RenderComponent>(entity_id);
		CModel* model = m_Engine->GetModel(render_component.myModelID);
		
		phys_body = component.myBody->InitAsBox(200, 100, 100, translation_component.myOrientation.GetPosition());// ->GetVertices(), model->GetIndices()); //InitAsBox(1, 1, 1, { 0,0,0 });

	}
	else if (shape == "box")
	{
		phys_body = component.myBody->InitAsBox(1, 1, 1, { 0,0,0 });
	}
	else if (shape == "sphere")
	{
		phys_body = component.myBody->InitAsSphere(
			1, //Radius
			object_mass, //Mass
			m_PhysicsManager->GetGravityForce(), //Gravity
			scientific_constants::pressure::air_pressure, //Air pressure / resistance
			translation_component.myOrientation.GetPosition()); //Start position
		
	}
	component.myBody->SetEntity(entity_id);
	m_PhysicsManager->Add(phys_body);
}

void LevelFactory::CreateCameraComponent(JSONReader& /*entity_reader*/, Entity entity_id)
{
	m_EntityManager->AddComponent<CameraComponent>(entity_id);
	CameraComponent& component = m_EntityManager->GetComponent<CameraComponent>(entity_id);
	m_DwellerList.GetLast()->AddComponent<CameraComponent>(&component, TreeDweller::CAMERA);

	component.m_Camera = Engine::GetInstance()->GetCamera();
}

void LevelFactory::CreateLightComponent(JSONReader& entity_reader, Entity entity_id, JSONElement::ConstMemberIterator it)
{
	m_EntityManager->AddComponent<LightComponent>(entity_id);
	LightComponent& component = m_EntityManager->GetComponent<LightComponent>(entity_id);
	m_DwellerList.GetLast()->AddComponent<LightComponent>(&component, TreeDweller::LIGHT);

	std::string type;


	if (entity_reader.DocumentHasMember("light"))
	{
		const JSONElement& el = entity_reader.GetElement("light");
		if (entity_reader.ElementHasMember(el, "type"))
		{
			type = el["type"].GetString();
		}
	}

	m_LevelReader.ReadElement(it->value["color"], component.color);
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

		m_LevelReader.ReadElement(it->value["direction"], component.direction);

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
	AIComponent& component = m_EntityManager->GetComponent<AIComponent>(entity_id);
	m_DwellerList.GetLast()->AddComponent<AIComponent>(&component, TreeDweller::AI);
	//read behaviour trees and stuff here...
}

void LevelFactory::CreateNetworkComponent(JSONReader& /*entity_reader*/, Entity entity_id)
{
	m_EntityManager->AddComponent<NetworkComponent>(entity_id);
	NetworkComponent& component = m_EntityManager->GetComponent<NetworkComponent>(entity_id);
	m_DwellerList.GetLast()->AddComponent<NetworkComponent>(&component, TreeDweller::NETWORK);

}

void LevelFactory::CreateDebugComponent(Entity e, bool isLight, s32 flags)
{
	m_EntityManager->AddComponent<DebugComponent>(e);
	DebugComponent& component = m_EntityManager->GetComponent<DebugComponent>(e);
	m_DwellerList.GetLast()->AddComponent<DebugComponent>(&component, TreeDweller::DEBUG);

	CU::Vector3f whd;
	if (!isLight)
	{
		RenderComponent& render = m_EntityManager->GetComponent<RenderComponent>(e);
		whd = m_Engine->GetModel(render.myModelID)->GetWHD();
		component.m_Rotation = render.m_Rotation;
	}
	else
	{
		whd = { 0.25f,0.25f, 0.25f };
	}
	TranslationComponent& translation = m_EntityManager->GetComponent<TranslationComponent>(e);
	CU::Vector3f pos = translation.myOrientation.GetPosition();

	float x = whd.x;
	float y = whd.y;
	float z = whd.z;
	component.m_WHD = whd;
	CU::Plane<float> plane0;

	CU::Vector4f up = translation.myOrientation.GetUp();
	CU::Vector4f right = translation.myOrientation.GetRight();
	CU::Vector4f forward = translation.myOrientation.GetForward();


	CU::Vector4f position;
	//x

	position = translation.myOrientation.GetTranslation();
	position += right * x;
	plane0.InitWithPointAndNormal(position, right);
	component.m_OBB.AddPlane(plane0);

	position = translation.myOrientation.GetTranslation();
	position -= right * x;
	right -= (right * 2.f);
	plane0.InitWithPointAndNormal(position, right);
	component.m_OBB.AddPlane(plane0);

	//y
	position = translation.myOrientation.GetTranslation();
	position += up * y;
	plane0.InitWithPointAndNormal(position, up);
	component.m_OBB.AddPlane(plane0);

	position = translation.myOrientation.GetTranslation();
	position -= up * y;
	up -= (up * 2.f);
	plane0.InitWithPointAndNormal(position, up);
	component.m_OBB.AddPlane(plane0);

	//z
	position = translation.myOrientation.GetTranslation();
	position += forward * z;
	plane0.InitWithPointAndNormal(position, forward);
	component.m_OBB.AddPlane(plane0);


	position = translation.myOrientation.GetTranslation();
	position -= forward * z;
	forward -= (forward * 2.f);
	plane0.InitWithPointAndNormal(position, forward);
	component.m_OBB.AddPlane(plane0);


	component.m_EditObject.Initiate(e, flags);

	component.m_PositionGizmo.SetPosition(pos);
	component.m_PositionGizmo.Initiate();
}

void LevelFactory::CreateTerrain(std::string terrain_path)
{
	CTerrain* terrain = Engine::GetInstance()->CreateTerrain(terrain_path, CU::Vector3f(0, 2, 0), CU::Vector2f(512, 512));
	//terrain->AddNormalMap("Data/Textures/t1_n.dds");
	/*
	Work([&](std::string texture) {
		CTerrain* terrain = m_Engine->CreateTerrain(texture, CU::Vector3f(0, 0, 0), CU::Vector2f(512, 512));
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


