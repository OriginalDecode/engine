#include "stdafx.h"
#include "CityGenerator.h"
#include <Engine/Engine.h>

#include <JSON/JSONReader.h>


static const char* s_LargeBuilding = "large";
static const char* s_MediumBuilding = "medium";
static const char* s_SmallBuilding = "small";
static const char* s_Size = "size";
static const char* s_Buildings = "buildings";
static const char* s_DefaultFile = "Data/GenContent/buildings.json";


CityGenerator* CityGenerator::m_Instance = nullptr;
void CityGenerator::Create()
{
	m_Instance = new  CityGenerator;
}

void CityGenerator::Destroy()
{
	delete m_Instance;
	m_Instance = nullptr;
}

CityGenerator::CityGenerator()
{
	//This is not used in final
	//read list of ModelInstances
	JSONReader reader;
	reader.OpenDocument(s_DefaultFile);
	
	const rapidjson::Document& doc = reader.GetDocument();
	for (auto& obj : doc.GetArray())
	{
		std::string _size = obj[s_Size].GetString();
		if (_size.find(s_LargeBuilding) != _size.npos)
		{
			CU::GrowingArray<CU::GrowingArray<ModelInstance>>& container = m_Instances[LARGE];
			for (const rapidjson::Value& value : obj[s_Buildings].GetArray())
			{
				CU::GrowingArray<ModelInstance> instances;
				JSONReader building_reader(value.GetString());
				auto& building_doc = building_reader.GetDocument();

				for (auto& building : building_doc.GetArray())
				{
					instances.Add(ModelInstance::Deserialize(building));
				}
				container.Add(instances);
			}
		}
	}


}


CityGenerator::~CityGenerator()
{
}

void CityGenerator::Generate(s8* map, const CU::Vector3f& center, s32 cm_per_pixel)
{

}

void CityGenerator::CreateSpline()
{
	assert(false && "no idea");
}

void CityGenerator::CreateEntity()
{
	EntityManager& manager = Engine::GetInstance()->GetEntityManager();
	Entity entity = manager.CreateEntity();

	GraphicsComponent& graphics = manager.AddComponent<GraphicsComponent>(entity);

	for (s32 i = 0; i < _COUNT; i++)
	{
		//Test fitting
		const s32 building = RANDOM(0, m_Instances[i].Size() - 1);
		graphics.m_Instances = m_Instances[i][building];

	}

	//graphics.m_Instances = RANDOM(0, m_);


}
