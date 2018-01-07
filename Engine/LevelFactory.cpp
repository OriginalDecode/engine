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


#include <JSON/include/writer.h>
#include <JSON/include/prettywriter.h>
#include <fstream>

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

	assert(cl::file_exist(entity_filepath) && "Failed to find file!");
	//FILE* pFile = fopen(entity_filepath.c_str(), "rb");
	//size_t size = ftell(pFile);
	//fseek(pFile, 0, SEEK_END);
	std::ifstream file(entity_filepath.c_str(), std::ios::binary);

	file.seekg(0, file.end);
	size_t length = file.tellg();
	file.seekg(0, file.beg);

	char* data = new char[length];
	file.read(data, length);
	file.close();

	int pos = 0;

	char ext[3] = { '\0' };
	memcpy(&ext[0], &data[pos], 3);
	pos += 3;
	
	int json_size = 0;
	memcpy(&json_size, &data[pos], sizeof(s32));
	pos += sizeof(s32);

	char* entity_data = new char[json_size];
	memcpy(&entity_data[0], &data[pos], json_size);
	pos += json_size;


	int physics_length = 0;
	memcpy(&physics_length, &data[pos], sizeof(int));
	pos += sizeof(int);


	char* physics_data = new char[physics_length];
	memcpy(&physics_data[0], &data[pos], physics_length);
	pos += physics_length;

	JSONReader reader;
	reader.OpenDocument(entity_data);
	auto& doc = reader.GetDocument();
	s32 debug_flags = 0;
	bool is_static = false;
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

		if (type.find("physics") != type.npos)
		{
			PhysicsComponent& c = m_EntityManager->AddComponent<PhysicsComponent>(e);
			c.Deserialize(obj);
			pDweller->AddComponent(&c, TreeDweller::PHYSICS);
			debug_flags |= TreeDweller::PHYSICS;

			TranslationComponent& t = m_EntityManager->GetComponent<TranslationComponent>(e);
			c.m_Body->DeserializePhysicsData(physics_data, physics_length, t.GetOrientation().GetPosition());
			btRigidBody* body = c.m_Body->GetBody();
			m_Engine->GetPhysicsManager()->Add(body);
			is_static = c.m_Body->IsStatic();

		}
	}


#ifdef _DEBUG
	CreateDebugComponent(e, false, debug_flags);
	DebugComponent& component = m_EntityManager->GetComponent<DebugComponent>(e);
	component.m_Dweller = pDweller;
#endif

	pDweller->Initiate(e, (is_static ? TreeDweller::STATIC : TreeDweller::DYNAMIC));
	delete[] physics_data;
	delete[] entity_data;
	delete[] data;
}

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

	/*if (pNet->IsHost())
	{
		PhysicsComponent& c = em.AddComponent<PhysicsComponent>(e);
		c.m_Body = engine->GetPhysicsManager()->CreateBody();

		Model* pModel = engine->GetModel<Model>(g_DefaultModel).GetData();
		pModel = pModel->GetChildModels()[0];
		//auto body = c.m_Body->InitWithMeshCollision(pModel->GetVertexWrapper().GetData(), pModel->GetIndexWrapper().GetData(), pModel->GetIndexWrapper().GetIndexCount(), pModel->GetVertexWrapper().GetVertexCount());
		auto body = c.m_Body->InitAsBox(0.5, 0.5, 0.5, { 0.f,0.f,0.f });
		engine->GetPhysicsManager()->Add(body);

		pDweller->AddComponent(&c, TreeDweller::PHYSICS);
		debug_flags |= TreeDweller::PHYSICS;
	}*/

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
		c.m_MinPoint = CU::Vector4f(engine->GetModel<Model>(g_DefaultModel)->GetMinPoint(), 1) * CU::Vector4f(1, 1, 1, 1);
		c.m_MaxPoint = CU::Vector4f(engine->GetModel<Model>(g_DefaultModel)->GetMaxPoint(), 1) * CU::Vector4f(1, 1, 1, 1);
	}
#endif

	pDweller->Initiate(e, TreeDweller::DYNAMIC);

#ifdef _DEBUG
	EventManager::GetInstance()->SendMessage(DebugEvents_AddEntity, pDweller);
#endif
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
		RefPointer<Model> model = m_Engine->GetModel<Model>(g.m_Instances[0].m_Filename.c_str());
		CU::Vector4f scale = g.m_Instances[0].m_Scale;
		if (scale.x <= 0.f && scale.y <= 0.f && scale.z <= 0.f)
		{
			scale.x = 1.f;
			scale.y = 1.f;
			scale.z = 1.f;
			scale.w = 1.f;
		}

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
	float height = 0.f;
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
		sprintf_s(buf, "%s%s%d.edb", folder.c_str(), _filename.c_str(), e);

		_writer.String(buf);

		writer.StartArray();

		bool has_physics = false;

		if (entity_manager.HasComponent<TranslationComponent>(e))
		{
			const TranslationComponent& c = entity_manager.GetComponent<TranslationComponent>(e);
			writer.StartObject();
			writer.String("component_type");
			writer.String("translation");
			c.Serialize(writer);
		}

		if (entity_manager.HasComponent<GraphicsComponent>(e))
		{
			const GraphicsComponent& c = entity_manager.GetComponent<GraphicsComponent>(e);
			writer.StartObject();
			writer.String("component_type");
			writer.String("graphics");
			c.Serialize(writer);
		}

		if (entity_manager.HasComponent<PhysicsComponent>(e))
		{
			const PhysicsComponent& c = entity_manager.GetComponent<PhysicsComponent>(e);
			has_physics = true;
			writer.StartObject();
			writer.String("component_type");
			writer.String("physics");
			c.Serialize(writer);

		}

		if (entity_manager.HasComponent<LightComponent>(e))
		{
			const LightComponent& c = entity_manager.GetComponent<LightComponent>(e);
			writer.StartObject();
			writer.String("component_type");
			writer.String("light");
			c.Serialize(writer);
		}

		writer.EndArray();


		FILE* pFile = fopen(buf, "wb");
		char ext[3] = { 'e', 'd', 'b' };
		fwrite(ext, 3, 1, pFile);
		int length = sb.GetLength();
		fwrite(&length, sizeof(int), 1, pFile);
		fwrite(sb.GetString(), length, 1, pFile);

		if (has_physics)
		{
			unsigned char* data = nullptr;
			int data_length = 0;
			PhysicsComponent& phys = entity_manager.GetComponent<PhysicsComponent>(e);
			RigidBody* body = phys.m_Body;
			body->SerializePhysicsData(data, data_length);
			assert(data != nullptr && "Serialized data was null");
			fwrite(&data_length, sizeof(int), 1, pFile);
			fwrite(data, data_length, 1, pFile);

		}

		fclose(pFile);

		//sprintf_s(buf, "%s%s%d.edb", folder.c_str(), _filename.c_str(), e);


		//_data.physics_length 

		//fwrite(sb.GetLength());


		//std::ofstream out(buf);
		//out << sb.GetString();
		//out.flush();
		//out.close();

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
