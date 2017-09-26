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
#include <PBLComponent.h>

void LevelFactory::Initiate()
{
	m_Engine = Engine::GetInstance();
	m_EntityManager = &m_Engine->GetEntityManager();
	m_PhysicsManager = m_Engine->GetPhysicsManager();
}

bool LevelFactory::CreateLevel(const std::string& level_path)
{
	/*if (!CL::substr(level_path, ".level"))
	{
		DL_ASSERT("Incorrect filetype. Has to be .level");
		return false;
	}*/


	//m_Engine->GetEntityManager()->CleanUp();
	//m_Engine->GetEntityManager()->Initiate();
	m_LevelReader.OpenDocument(level_path);

	const JSONElement& el = m_LevelReader.GetElement("root");

	CreateTerrain("Data/Textures/flat_height.tga");
	//m_Engine->GetThreadpool().AddWork(Work([&]() {CreateTerrain("Data/Textures/flat_height.tga"); }));

	for (JSONElement::ConstMemberIterator it = el.MemberBegin(); it != el.MemberEnd(); it++)
	{
		CreateEntitiy(it->value["entity"].GetString(), it);
	}

	return true;
}

void LevelFactory::CreateEntitiy(const std::string& entity_filepath, JSONElement::ConstMemberIterator it)
{
	std::string data_path = "Data/Levels/";
#define DATA_PATH data_path +

	JSONReader entity_reader(DATA_PATH entity_filepath);
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

	if (sponza)
		sponza = false;
	//#ifdef _EDITOR
		//if(hasLight)
			//CreateDebugComponent(e, hasLight, debug_flags);
	//#endif

	TranslationComponent& component = m_EntityManager->GetComponent<TranslationComponent>(e);

	/*CU::Vector3f new_pos = pos;
	new_pos.y += 5.f;
	new_pos.x += 400.f;
	new_pos.z += 400.f;*/

	//component.myOrientation.SetPosition(new_pos);


	m_DwellerList.GetLast()->Initiate(e, TreeDweller::STATIC);

	//TranslationComponent& translation = m_EntityManager->GetComponent<TranslationComponent>(e);

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

	dweller->Initiate(e, TreeDweller::eType::STATIC);
	return dweller;
}


void LevelFactory::CreateTranslationComponent(Entity entity_id, const CU::Vector3f& position)
{
	m_EntityManager->AddComponent<TranslationComponent>(entity_id);

	TranslationComponent& component = m_EntityManager->GetComponent<TranslationComponent>(entity_id);
	component.myOrientation.SetPosition(position);
	m_DwellerList.GetLast()->AddComponent<TranslationComponent>(&component, TreeDweller::TRANSLATION);
}

void LevelFactory::CreateGraphicsComponent(JSONReader& entity_reader, Entity entity_id, JSONElement::ConstMemberIterator it)
{
	m_EntityManager->AddComponent<RenderComponent>(entity_id);
	RenderComponent& component = m_EntityManager->GetComponent<RenderComponent>(entity_id);

	const JSONElement& el = entity_reader.GetElement("graphics");
	CU::Vector3f scale;
	m_LevelReader.ReadElement(it->value["scale"], scale);
	CU::Vector3f rotation;
	m_LevelReader.ReadElement(it->value["rotation"], rotation);

	if (!el.IsArray())
	{
		component.m_ModelID = m_Engine->LoadModel(
			el["model"].GetString(),
			el["shader"].GetString(),
			true);


		if (el["model"] == "Data/Model/sponza/Sponza_2.fbx")
			sponza = true;
		component.m_MinPos = m_Engine->GetModel(component.m_ModelID)->GetMinPoint();
		component.m_MaxPos = m_Engine->GetModel(component.m_ModelID)->GetMaxPoint();



		component.m_Rotation = rotation;

		TranslationComponent& translation = m_EntityManager->GetComponent<TranslationComponent>(entity_id);
		translation.myOrientation = CU::Matrix44f::CreateRotateAroundZ(cl::DegreeToRad(rotation.z)) * translation.myOrientation;
		translation.myOrientation = CU::Matrix44f::CreateRotateAroundX(cl::DegreeToRad(rotation.x)) * translation.myOrientation;
		translation.myOrientation = CU::Matrix44f::CreateRotateAroundY(cl::DegreeToRad(rotation.y)) * translation.myOrientation;
		//translation.myOrientation = translation.myOrientation * CU::Matrix44f::CreateRotateAroundX(CL::DegreeToRad(rotation.x));
		//translation.myOrientation = translation.myOrientation * CU::Matrix44f::CreateRotateAroundZ(CL::DegreeToRad(rotation.z));
		//translation.myOrientation = translation.myOrientation * CU::Matrix44f::CreateRotateAroundY(CL::DegreeToRad(rotation.y));

		component.scale = scale;
		component.scale.w = 1.f;

		CU::Vector3f whd = m_Engine->GetModel(component.m_ModelID)->GetWHD();
		m_DwellerList.GetLast()->AddComponent<RenderComponent>(&component, TreeDweller::GRAPHICS);
		m_DwellerList.GetLast()->SetWHD(whd);
	}
	else
	{
		for (rapidjson::SizeType i = 0; i < el.Size(); i++)
		{
			ModelInstance instance;


			const auto& obj = el[i];
			auto key_value = obj["key"].GetString();
			auto shader = obj["shader"].GetString();

			CU::Vector3f rel_pos;
			entity_reader.ReadElement(obj["relative_position"], rel_pos);

			CU::Vector3f rel_scale;
			entity_reader.ReadElement(obj["relative_scale"], rel_scale);

			CU::Vector3f rel_rot;
			entity_reader.ReadElement(obj["relative_rotation"], rel_rot);

			instance.m_ModelID = m_Engine->LoadModel(key_value, shader, true);

			instance.m_Orientation.SetPosition(rel_pos);
			CU::Matrix44f t = instance.m_Orientation;
			instance.m_Orientation = CU::Matrix44f::CreateScaleMatrix(CU::Vector4f(rel_scale, 1.f)) * t;

			instance.m_Orientation = CU::Matrix44f::CreateRotateAroundZ(cl::DegreeToRad(rotation.z)) * instance.m_Orientation;
			instance.m_Orientation = CU::Matrix44f::CreateRotateAroundX(cl::DegreeToRad(rotation.x)) * instance.m_Orientation;
			instance.m_Orientation = CU::Matrix44f::CreateRotateAroundY(cl::DegreeToRad(rotation.y)) * instance.m_Orientation;

			component.m_Instances.Add(instance);


			component.scale = scale;
			component.scale.w = 1.f;

			CU::Vector3f whd = m_Engine->GetModel(instance.m_ModelID)->GetWHD();
			m_DwellerList.GetLast()->AddComponent<RenderComponent>(&component, TreeDweller::GRAPHICS);
			m_DwellerList.GetLast()->SetWHD(whd);
		}
	}
}

void LevelFactory::CreateGraphicsComponent(JSONReader& entity_reader, Entity entity_id)
{
	m_EntityManager->AddComponent<RenderComponent>(entity_id);

	RenderComponent& component = m_EntityManager->GetComponent<RenderComponent>(entity_id);
	const JSONElement& el = entity_reader.GetElement("graphics");
	component.m_ModelID = m_Engine->LoadModel(
		el["model"].GetString(),
		el["shader"].GetString(),
		true);
	component.m_MinPos = m_Engine->GetModel(component.m_ModelID)->GetMinPoint();
	component.m_MaxPos = m_Engine->GetModel(component.m_ModelID)->GetMaxPoint();

	component.scale = CU::Vector4f(1, 1, 1, 1);
	component.m_Shadowed = false;

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
		//RenderComponent& render_component = m_EntityManager->GetComponent<RenderComponent>(entity_id);
		//CModel* model = m_Engine->GetModel(render_component.myModelID);

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
	// 	m_EntityManager->AddComponent<CameraComponent>(entity_id);
	// 	CameraComponent& component = m_EntityManager->GetComponent<CameraComponent>(entity_id);
	// 	m_DwellerList.GetLast()->AddComponent<CameraComponent>(&component, TreeDweller::CAMERA);
	// 
	// 	component.m_Camera = Engine::GetInstance()->GetCamera();
}

void LevelFactory::CreateLightComponent(JSONReader& entity_reader, Entity entity_id, JSONElement::ConstMemberIterator it)
{
	m_EntityManager->AddComponent<LightComponent>(entity_id);
	LightComponent& component = m_EntityManager->GetComponent<LightComponent>(entity_id);
	m_DwellerList.GetLast()->AddComponent<LightComponent>(&component, TreeDweller::LIGHT);
	//TranslationComponent& translation_component = m_EntityManager->GetComponent<TranslationComponent>(entity_id);

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

		TranslationComponent& translation = m_EntityManager->GetComponent<TranslationComponent>(entity_id);
		CU::Vector3f rotation;
		m_LevelReader.ReadElement(it->value["rotation"], rotation);

		translation.myOrientation = CU::Matrix44f::CreateRotateAroundZ(cl::DegreeToRad(rotation.z)) * translation.myOrientation;
		translation.myOrientation = CU::Matrix44f::CreateRotateAroundX(cl::DegreeToRad(rotation.x)) * translation.myOrientation;
		translation.myOrientation = CU::Matrix44f::CreateRotateAroundY(cl::DegreeToRad(rotation.y)) * translation.myOrientation;

		//translation_component.myOrientation = CU::Matrix44f::CreateRotateAroundZ(CL::DegreeToRad(component.direction.z)) * translation_component.myOrientation;
		//translation_component.myOrientation = CU::Matrix44f::CreateRotateAroundY(CL::DegreeToRad(component.direction.y)) * translation_component.myOrientation;
		//translation_component.myOrientation = CU::Matrix44f::CreateRotateAroundX(CL::DegreeToRad(component.direction.x)) * translation_component.myOrientation;

		m_LevelReader.ReadElement(it->value["angle"], component.angle);
		component.angle = cl::DegreeToRad(component.angle);
		component.m_LightID = Engine::GetInstance()->RegisterLight();
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
		Model* model = m_Engine->GetModel(render.m_ModelID);
		whd = model->GetWHD();
		component.m_Rotation = render.m_Rotation;
		component.m_MinPoint = model->GetMinPoint();
		component.m_MaxPoint = model->GetMaxPoint();


	}
	else
	{
		whd = { 0.25f,0.25f, 0.25f };
		component.m_MinPoint = { -0.25,-0.25,-0.25 };
		component.m_MaxPoint = { 0.25,0.25,0.25 };

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

	{
		PositionGizmo& position_gizmo = component.m_PositionGizmo;
		GizmoHandle& gizmo_up = position_gizmo.GetUp();
		GizmoHandle& gizmo_right = position_gizmo.GetRight();
		GizmoHandle& gizmo_forward = position_gizmo.GetForward();

		gizmo_up.m_Offset = 1.5f;
		gizmo_right.m_Offset = 1.5f;
		gizmo_forward.m_Offset = 1.5f;

		position_gizmo.SetPosition(pos);
		position_gizmo.CreateGizmoHandle(gizmo_up, "Data/Model/green_arrow.fbx", "Data/Textures/green.dds", GizmoHandle::eDirection::UP);
		gizmo_up.m_Orientation = CU::Matrix44f::CreateRotateAroundZ(cl::DegreeToRad(90.f) * 1) * gizmo_up.m_Orientation;

		position_gizmo.CreateGizmoHandle(gizmo_right, "Data/Model/red_arrow.fbx", "Data/Textures/red.dds", GizmoHandle::eDirection::RIGHT);

		position_gizmo.CreateGizmoHandle(gizmo_forward, "Data/Model/blue_arrow.fbx", "Data/Textures/blue.dds", GizmoHandle::eDirection::FORWARD);
		gizmo_forward.m_Orientation = CU::Matrix44f::CreateRotateAroundY(cl::DegreeToRad(90.f) * -1) * gizmo_forward.m_Orientation;
	}


	{
		RotationGizmo& rotation_gizmo = component.m_RotationGizmo;
		GizmoHandle& gizmo_up = rotation_gizmo.GetUp();
		GizmoHandle& gizmo_right = rotation_gizmo.GetRight();
		GizmoHandle& gizmo_forward = rotation_gizmo.GetForward();

		gizmo_right.m_Offset = 0.5f;
		gizmo_forward.m_Offset = 0.5f;
		rotation_gizmo.SetPosition(pos);

		CU::Vector3f up_pos = pos;
		up_pos.x += 0.5f;
		up_pos.y -= 0.5f;
		up_pos.z += 0.5f;
		gizmo_up.SetPosition(up_pos);

		rotation_gizmo.CreateGizmoHandle(gizmo_up, "Data/Model/rotate_y.fbx", "Data/Textures/green.dds", GizmoHandle::eDirection::Y);
		gizmo_up.m_Orientation = CU::Matrix44f::CreateRotateAroundY(cl::DegreeToRad(90.f) * -1) * gizmo_up.m_Orientation;
		gizmo_up.m_Orientation = CU::Matrix44f::CreateRotateAroundX(cl::DegreeToRad(90.f) * -1) * gizmo_up.m_Orientation;

		rotation_gizmo.CreateGizmoHandle(gizmo_right, "Data/Model/rotate_x.fbx", "Data/Textures/red.dds", GizmoHandle::eDirection::X);

		rotation_gizmo.CreateGizmoHandle(gizmo_forward, "Data/Model/rotate_z.fbx", "Data/Textures/blue.dds", GizmoHandle::eDirection::Z);
		gizmo_forward.m_Orientation = CU::Matrix44f::CreateRotateAroundY(cl::DegreeToRad(90.f) * -1) * gizmo_forward.m_Orientation;
		rotation_gizmo.ToggleActive();


	}



	//component.m_PositionGizmo.Initiate();
}

void LevelFactory::CreateTerrain(std::string terrain_path)
{
	/*Terrain* terrain = */m_Engine->CreateTerrain(terrain_path, CU::Vector3f(0, 0, 0), CU::Vector2f(512, 512));
	return;
	float uniform_height = -4;
	Terrain* terrain = m_Engine->CreateTerrain("Data/Textures/t_0.tga", CU::Vector3f(0, uniform_height, 0), CU::Vector2f(512, 512));
	terrain->AddNormalMap("Data/Textures/t0_n.dds");
	terrain = m_Engine->CreateTerrain("Data/Textures/t_1.tga", CU::Vector3f(0, uniform_height, 510), CU::Vector2f(512, 512));
	terrain->AddNormalMap("Data/Textures/t1_n.dds");
	terrain = m_Engine->CreateTerrain("Data/Textures/t_2.tga", CU::Vector3f(510, uniform_height, 0), CU::Vector2f(512, 512));
	terrain->AddNormalMap("Data/Textures/t2_n.dds");
	terrain = m_Engine->CreateTerrain("Data/Textures/t_3.tga", CU::Vector3f(510, uniform_height, 510), CU::Vector2f(512, 512));
	terrain->AddNormalMap("Data/Textures/t3_n.dds");
	//m_Engine->GetThreadpool().AddWork(Work([=] {
	//}));
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


void LevelFactory::CreatePBLLevel(s32 steps)
{
	
	CreateTerrain("Data/Textures/flat_height.tga");
	float height = 1.f;
	float x_start = 5.f;
	float z_start = 5.f;
	float metal = 0.f;
	for (s32 i = 0; i < steps; i++)
	{
		for (s32 j = steps, s = 0; j >= 0; j--, s++)
		{
			metal = (float)i / steps;
			Entity e = m_EntityManager->CreateEntity();
			auto& pbl = m_EntityManager->AddComponent<PBLComponent>(e);

			pbl.m_PBLValues.m_Metalness = metal;
			pbl.m_PBLValues.m_Roughness = (float)j / steps;

			//pbl.m_Buffer = m_Engine->GetAPI()->CreateConstantBuffer(sizeof(pbl.m_PBLValues));


			auto& t = m_EntityManager->AddComponent<TranslationComponent>(e);
			auto& r = m_EntityManager->AddComponent<RenderComponent>(e);

			CU::Vector4f translation;
			translation.x = x_start + i * 15.f;
			translation.y = height;
			translation.z = z_start + s * 15.f;
			translation.w = 1.f;

			t.myOrientation.SetTranslation(translation);


			/*std::string key = Engine::GetInstance()->LoadModel("Data/Model/ballen.fbx", "Shaders/debug_pbl.json", false);
			Model* m = m_Engine->GetModel(key);*/
			//m->AddTexture("Data/Textures/brickwall_normal.dds", Effect::NORMAL);

			/*
			auto& c = m->GetChildModels(); 
			for (Model* child : c)
			{
				auto& s = child->GetSurfaces();

				for (Surface* surface : s)
				{
					surface->AddTexture("Data/Textures/brickwall_normal.dds", Effect::NORMAL);
				}
			}
			*/
/*
			r.myModelID = key;
			r.scale = CU::Vector4f(1, 1, 1, 1);*/

			m_DwellerList.Add(new TreeDweller);
			m_DwellerList.GetLast()->AddComponent(&t, TreeDweller::TRANSLATION);
			m_DwellerList.GetLast()->AddComponent(&r, TreeDweller::GRAPHICS);
			m_DwellerList.GetLast()->AddComponent(&pbl, TreeDweller::PBL);
			m_DwellerList.GetLast()->Initiate(e, TreeDweller::STATIC);
		}

	}

}
