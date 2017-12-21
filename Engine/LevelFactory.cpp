#include "stdafx.h"
#include "LevelFactory.h"

#include "Engine.h"

#include <EntityManager.h>
#include <PhysicsManager.h>

#include "Terrain.h"

//these should probably be moved
#include <PhysicsComponent.h>
#include <GraphicsComponent.h>
#include <TranslationComponent.h>
#include <DebugComponent.h>
#include <LightComponent.h>
#include <InputComponent.h>
#include <AIComponent.h>
#include <NetworkComponent.h>
#include <CameraComponent.h>
//these should probably be moved

#include <RigidBody.h>
#include <GhostObject.h>

#include "TreeDweller.h"

#include <Engine/AssetFactory.h>
#include <Engine/Material.h>

#ifdef _DEBUG
#include "hash/DebugEvents.h"
#include <PostMaster/EventManager.h>
#endif

#include <network/NetworkManager.h>
#include <network/NetCreateEntity.h>

void LevelFactory::Initiate()
{
	m_Engine = Engine::GetInstance();
	m_EntityManager = &m_Engine->GetEntityManager();
	m_PhysicsManager = m_Engine->GetPhysicsManager();
}

bool LevelFactory::CreateLevel(const std::string& level_path)
{
	m_LevelReader.OpenDocument(level_path);
	const rapidjson::Document& doc = m_LevelReader.GetDocument();
	for (auto& obj : doc.GetArray())
	{
		CreateEntity(obj.GetString());
	}

	CreateTerrain("Data/Textures/flat_height.tga");
	m_LevelReader.CloseDocument();
	//m_Engine->GetThreadpool().AddWork(Work([&]() {CreateTerrain("Data/Textures/flat_height.tga"); }));

	return true;
}

void LevelFactory::CreateEntity(const std::string& entity_filepath)
{
	m_DwellerList.Add(new TreeDweller);
	TreeDweller* pDweller = m_DwellerList.GetLast();

	Entity e = m_EntityManager->CreateEntity();
	JSONReader reader(entity_filepath);
	auto& doc = reader.GetDocument();
	s32 debug_flags = 0;
	for (const rapidjson::Value& obj : doc.GetArray())
	{
		std::string type = obj["component_type"].GetString();

		if (type.find("translation") != type.npos)
		{
			TranslationComponent& c = m_EntityManager->AddComponent<TranslationComponent>(e);
			c.Deserialize(obj);
			pDweller->AddComponent(&c, TreeDweller::TRANSLATION);
			debug_flags |= TreeDweller::TRANSLATION;
		}

		if (type.find("graphics") != type.npos)
		{
			GraphicsComponent& c = m_EntityManager->AddComponent<GraphicsComponent>(e);
			c.Deserialize(obj);
			pDweller->AddComponent(&c, TreeDweller::GRAPHICS);
			debug_flags |= TreeDweller::GRAPHICS;
		}

		if (type.find("light") != type.npos)
		{
			LightComponent& c = m_EntityManager->AddComponent<LightComponent>(e);
			c.Deserialize(obj);
			pDweller->AddComponent(&c, TreeDweller::LIGHT);
			debug_flags |= TreeDweller::LIGHT;
		}

#ifdef _DEBUG

		CreateDebugComponent(e, false, debug_flags);
		DebugComponent& component = m_EntityManager->GetComponent<DebugComponent>(e);
		component.m_Dweller = pDweller;
#endif

	}
	reader.CloseDocument();
	pDweller->Initiate(e, TreeDweller::STATIC);
}


//https://stackoverflow.com/questions/6849287/c-callback-how-to-decouple-the-call-back-type
void LevelFactory::CreateEntity(Entity e, EntityManager& em)
{
	Engine* engine = Engine::GetInstance();
	network::NetworkManager* pNet = engine->GetNetworkManager();

	//m_DwellerList.Add(new TreeDweller);
	TreeDweller* pDweller = new TreeDweller; // m_DwellerList.GetLast();
	s32 debug_flags = 0;

	{
		TranslationComponent& c = em.AddComponent<TranslationComponent>(e);
		pDweller->AddComponent(&c, TreeDweller::TRANSLATION);
		debug_flags |= TreeDweller::TRANSLATION;
	}

	{
		GraphicsComponent& c = em.AddComponent<GraphicsComponent>(e);
		ModelInstance instance;
		instance.m_MaterialKey = g_DefaultMaterial; //Engine::GetInstance()->GetMaterial("default")->GetKey(); // this will work
		instance.m_MaterialFile = "default";
		instance.m_ModelID = g_DefaultModel;
		instance.m_Filename = "default";
		instance.m_Scale = { 1.f, 1.f, 1.f, 1.f };
		c.m_Instances.Add(instance);
		pDweller->AddComponent(&c, TreeDweller::GRAPHICS);
		debug_flags |= TreeDweller::GRAPHICS;
	}

	if (pNet->IsHost())
	{
		PhysicsComponent& c = em.AddComponent<PhysicsComponent>(e);
		c.m_Body = engine->GetPhysicsManager()->CreateBody();

		Model* pModel = engine->GetModel(g_DefaultModel);
		pModel = pModel->GetChildModels()[0];
		//auto body = c.m_Body->InitWithMeshCollision(pModel->GetVertexWrapper().GetData(), pModel->GetIndexWrapper().GetData(), pModel->GetIndexWrapper().GetIndexCount(), pModel->GetVertexWrapper().GetVertexCount());
		auto body = c.m_Body->InitAsBox(0.5, 0.5, 0.5, { 0.f,0.f,0.f });
		engine->GetPhysicsManager()->Add(body);

		pDweller->AddComponent(&c, TreeDweller::PHYSICS);
		debug_flags |= TreeDweller::PHYSICS;
	}

	{
		//This is only if the entity is local.
		NetworkComponent& c = em.AddComponent<NetworkComponent>(e);
		if (pNet->IsHost())
		{
			network::CreateGUID(&c.m_GUID);
			c.m_Owner = pNet->GetGUID();
			Engine::GetInstance()->GetNetworkManager()->Send(NetCreateEntity(c.m_GUID));
		}
		

		pDweller->AddComponent(&c, TreeDweller::NETWORK);
		debug_flags |= TreeDweller::NETWORK;
	}
#ifdef _DEBUG
	{
		DebugComponent& c = em.AddComponent<DebugComponent>(e);
		pDweller->AddComponent(&c, TreeDweller::DEBUG);
		c.m_ComponentFlags = debug_flags;
		c.m_Dweller = pDweller;
		c.m_MinPoint = CU::Vector4f(engine->GetModel(g_DefaultModel)->GetMinPoint(), 1) * CU::Vector4f(1, 1, 1, 1);
		c.m_MaxPoint = CU::Vector4f(engine->GetModel(g_DefaultModel)->GetMaxPoint(), 1) * CU::Vector4f(1, 1, 1, 1);
	}
#endif

	pDweller->Initiate(e, TreeDweller::STATIC);

#ifdef _DEBUG
	EventManager::GetInstance()->SendMessage(DebugEvents_AddEntity, pDweller);
#endif
}

void LevelFactory::CreatePhysicsComponent(JSONReader& entity_reader, Entity entity_id)
{
	m_EntityManager->AddComponent<PhysicsComponent>(entity_id);

	PhysicsComponent& component = m_EntityManager->GetComponent<PhysicsComponent>(entity_id);
	m_DwellerList.GetLast()->AddComponent<PhysicsComponent>(&component, TreeDweller::PHYSICS);

	const JSONElement& el = entity_reader.GetElement("physics");
	float object_mass = (float)el["mass"].GetDouble();

	component.m_Body = m_PhysicsManager->CreateBody();
	btRigidBody* phys_body = nullptr;

	TranslationComponent& translation_component = m_EntityManager->GetComponent<TranslationComponent>(entity_id);



	std::string shape = el["shape"].GetString();
	if (shape == "mesh")
	{
		//RenderComponent& render_component = m_EntityManager->GetComponent<RenderComponent>(entity_id);
		//CModel* model = m_Engine->GetModel(render_component.myModelID);

		phys_body = component.m_Body->InitAsBox(200, 100, 100, translation_component.GetOrientation().GetPosition());// ->GetVertices(), model->GetIndices()); //InitAsBox(1, 1, 1, { 0,0,0 });

	}
	else if (shape == "box")
	{
		phys_body = component.m_Body->InitAsBox(1, 1, 1, { 0,0,0 });
	}
	else if (shape == "sphere")
	{
		phys_body = component.m_Body->InitAsSphere(
			1, //Radius
			object_mass, //Mass
			m_PhysicsManager->GetGravityForce(), //Gravity
			scientific_constants::pressure::air_pressure, //Air pressure / resistance
			translation_component.GetOrientation().GetPosition()); //Start position

	}
	component.m_Body->SetEntity(entity_id);
	m_PhysicsManager->Add(phys_body);
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

	CU::Vector3f col;
	m_LevelReader.ReadElement(it->value["color"], col);
	m_LevelReader.ReadElement(it->value["range"], component.range);

	if (col > CU::Vector3f(1.f, 1.f, 1.f))
		col /= 255.f;

	component.color = col;
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

		//translation.myOrientation = CU::Matrix44f::CreateRotateAroundX(cl::DegreeToRad(rotation.x)) * translation.myOrientation;
		//translation.myOrientation = CU::Matrix44f::CreateRotateAroundY(cl::DegreeToRad(rotation.y)) * translation.myOrientation;
		//translation.myOrientation = CU::Matrix44f::CreateRotateAroundZ(cl::DegreeToRad(rotation.z)) * translation.myOrientation;

		m_LevelReader.ReadElement(it->value["angle"], component.angle);
		component.angle = cl::DegreeToRad(component.angle);
		component.m_LightID = Engine::GetInstance()->RegisterLight();
	}

}

void LevelFactory::CreateDebugComponent(Entity e, bool isLight, s32 flags)
{
	m_EntityManager->AddComponent<DebugComponent>(e);
	DebugComponent& component = m_EntityManager->GetComponent<DebugComponent>(e);
	m_DwellerList.GetLast()->AddComponent<DebugComponent>(&component, TreeDweller::DEBUG);
	component.m_ComponentFlags = flags;
	if (!isLight && m_EntityManager->HasComponent<GraphicsComponent>(e))
	{
		GraphicsComponent& g = m_EntityManager->GetComponent<GraphicsComponent>(e);
		Model* model = m_Engine->GetModel(g.m_Instances[0].m_Filename.c_str());
		CU::Vector4f scale = g.m_Instances[0].m_Scale;
		if (scale.x <= 0.f && scale.y <= 0.f && scale.z <= 0.f)
		{
			scale.x = 1.f;
			scale.y = 1.f;
			scale.z = 1.f;
			scale.w = 1.f;
		}

		//component.m_Rotation = g.m_Rotation;
		component.m_MinPoint = CU::Vector4f(model->GetMinPoint(), 1) * scale;
		component.m_MaxPoint = CU::Vector4f(model->GetMaxPoint(), 1) * scale;
	}
	else
	{
		component.m_MinPoint = { -0.25,-0.25,-0.25 };
		component.m_MaxPoint = { 0.25,0.25,0.25 };
	}

}

void LevelFactory::CreateTerrain(std::string terrain_path)
{
	Terrain* terrain = m_Engine->CreateTerrain(terrain_path, CU::Vector3f(0, 0, 0), CU::Vector2f(512, 512));
	Material* pGroundMaterial = m_Engine->GetMaterial("Data/Material/mat_grass.json");
	terrain->SetMaterial(pGroundMaterial);
}

void LevelFactory::CreatePBLLevel(s32 steps)
{
	CreateTerrain("Data/Textures/flat_height.tga");
	float height = 1.f;
	float x_start = 5.f;
	float z_start = 5.f;

	Engine* pEngine = Engine::GetInstance();
	Material* pGoldMaterial = pEngine->GetMaterial("Data/Material/mat_gold.json");
	Material* pAlumMaterial = pEngine->GetMaterial("Data/Material/mat_aluminum.json");
	Material* pCoppMaterial = pEngine->GetMaterial("Data/Material/mat_copper.json");
	Material* pMetaMaterial = pEngine->GetMaterial("Data/Material/mat_metal.json");
	Material* pStoneMaterial = pEngine->GetMaterial("Data/Material/mat_octostone.json");

	Material* material[] = {
		pStoneMaterial,
		pGoldMaterial,
		pAlumMaterial,
		pCoppMaterial,
		pMetaMaterial,
	};

	const char* files[] = {
		"Data/Material/mat_gold.json",
		"Data/Material/mat_aluminum.json",
		"Data/Material/mat_copper.json",
		"Data/Material/mat_metal.json",
		"Data/Material/mat_octostone.json",
	};


	for (s32 i = 0; i < steps; i++)
	{
		for (s32 j = steps - 1, s = 0; j >= 0; j--, s++)
		{
			Entity e = m_EntityManager->CreateEntity();

			auto& t = m_EntityManager->AddComponent<TranslationComponent>(e);
			auto& r = m_EntityManager->AddComponent<GraphicsComponent>(e);

			CU::Vector4f translation;
			translation.x = x_start + i * 15.f;
			translation.y = height;
			translation.z = z_start + s * 15.f;
			translation.w = 1.f;

			//t.SetOrientation(translation);

			auto v = RANDOM(0, ARRSIZE(material));

			auto key = Engine::GetInstance()->LoadModel<Model>("Data/Model/ballen.fbx", "Shaders/debug_pbl_instanced.json", false);

			ModelInstance instance;
			instance.m_Filename = "data/model/ballen.fbx";
			instance.m_MaterialFile = files[v];

			Material* pMaterial = material[v];
			pMaterial->SetEffect(pEngine->GetEffect("Shaders/debug_pbl_instanced.json"));

			instance.m_MaterialKey = pMaterial->GetKey();
			instance.m_ModelID = key;

			r.m_Scale = CU::Vector4f(1, 1, 1, 1);

			r.m_Instances.Add(instance);

			m_DwellerList.Add(new TreeDweller);
			m_DwellerList.GetLast()->AddComponent(&t, TreeDweller::TRANSLATION);
			m_DwellerList.GetLast()->AddComponent(&r, TreeDweller::GRAPHICS);
			m_DwellerList.GetLast()->Initiate(e, TreeDweller::STATIC);
		}

	}
}

#include <JSON/include/writer.h>
#include <JSON/include/prettywriter.h>
#include <fstream>
void LevelFactory::SaveLevel(std::string folder, std::string filename) //Should be a static function.
{

	Engine* pEngine = Engine::GetInstance();
	EntityManager& entity_manager = pEngine->GetEntityManager();
	const EntityArray& entities = entity_manager.GetEntities();
	rapidjson::StringBuffer _sb;
	rapidjson::PrettyWriter<decltype(_sb)> _writer(_sb);

	_writer.StartArray();

	std::string _filename = "entity_";
	for (Entity e : entities)
	{
		rapidjson::StringBuffer sb;
		rapidjson::PrettyWriter<decltype(sb)> writer(sb);
		char buf[100];
		ZeroMemory(buf, sizeof(buf));
		//memset(buf, 0, sizeof(buf));
		sprintf_s(buf, "%s%s%d.json", folder.c_str(), _filename.c_str(), e);
		_writer.String(buf);


		writer.StartArray();

		if (entity_manager.HasComponent<TranslationComponent>(e))
		{
			const TranslationComponent& c = entity_manager.GetComponent<TranslationComponent>(e);
			c.Serialize(writer);
		}

		if (entity_manager.HasComponent<GraphicsComponent>(e))
		{
			const GraphicsComponent& c = entity_manager.GetComponent<GraphicsComponent>(e);
			c.Serialize(writer);
		}

		if (entity_manager.HasComponent<PhysicsComponent>(e))
		{
			const PhysicsComponent& c = entity_manager.GetComponent<PhysicsComponent>(e);
			c.Serialize(writer);
		}

		if (entity_manager.HasComponent<LightComponent>(e))
		{
			const LightComponent& c = entity_manager.GetComponent<LightComponent>(e);
			c.Serialize(writer);
		}


		writer.EndArray();


		std::ofstream out(buf);
		out << sb.GetString();
		out.flush();
		out.close();

	}
	_writer.EndArray();

	std::string out_file = folder + filename;



	std::stringstream ss;
	ss << _sb.GetString();
	OutputDebugString(ss.str().c_str());

	std::ofstream out(out_file);
	out << _sb.GetString();
	out.flush();
	out.close();


}
