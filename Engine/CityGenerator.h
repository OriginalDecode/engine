#pragma once
#include <Engine/engine_shared.h>
#include <EntitySystem/GraphicsComponent.h>

class CityGenerator
{
public:

	enum BuildingType
	{
		LARGE,
		MEDIUM,
		SMALL,
		_COUNT
	};


	void Generate(s8* map, const CU::Vector3f& center, s32 cm_per_pixel);  //This will spit out a list or object that contains all the information needed

	static void Create();
	static void Destroy();
	static CityGenerator& Get() { return *m_Instance; }
	static CityGenerator* GetInstance() { return m_Instance; }
private:
	static CityGenerator* m_Instance;

	CityGenerator();
	~CityGenerator();


	void CreateSpline();
	void CreateEntity(); 

	CU::GrowingArray<ModelInstance> m_Instances[_COUNT]; //variations;



};

