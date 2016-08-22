#include "Game.h"

/* Entity System Includes */
#include <EntityManager.h>
/* Components */
#include <PhysicsComponent.h>
#include <RenderComponent.h>
#include <TranslationComponent.h>
#include <LightComponent.h>
/* Systems */
#include <PhysicsSystem.h>
#include <RenderSystem.h>
#include <LightSystem.h>
/* End of Entity System Includes*/
/* Physics */
#include <PhysicsManager.h>
#include <RigidBody.h>

/* Physics End */

#include <Synchronizer.h>
#include <RenderCommand.h>
#include <Line3D.h>

#include <AssetsContainer.h>
#include <EngineDefines.h>
#include <JSON/JSONReader.h>
#include <DL_Debug.h>
#include <Engine.h>
#include <Terrain.h>

CGame::CGame(Snowblind::CSynchronizer* aSynchronizer)
	: mySynchronizer(aSynchronizer)
{
	myEntityManager = new CEntityManager();
	myPhysicsManager = new CPhysicsManager();
	myEngine = Snowblind::CEngine::GetInstance();

	myEngine->ToggleVsync();

	JSONReader reader("Data/Levels/level_01.json");

	//3d picking
	myTerrain.Add(myEngine->CreateTerrain("Data/Textures/T_heightmap_level_00.tga", CU::Vector3f(0, 0, 0), CU::Vector2f(512, 512)));
	myTerrain.Add(myEngine->CreateTerrain("Data/Textures/playground.tga", CU::Vector3f(512, 0, 0), CU::Vector2f(512, 512)));
	myTerrain.Add(myEngine->CreateTerrain("Data/Textures/T_heightmap_level_00.tga", CU::Vector3f(512, 0, 512), CU::Vector2f(512, 512)));
	myTerrain.Add(myEngine->CreateTerrain("Data/Textures/T_heightmap_level_00.tga", CU::Vector3f(0, 0, 512), CU::Vector2f(512, 512)));

	for (u32 i = 0; i < 1; i++)
	{
		myTerrainBodies.Add(myPhysicsManager->CreateBody());
		myPhysicsManager->Add(myTerrainBodies[i]->InitAsTerrain(myTerrain[i]->GetVerticeArrayCopy(), myTerrain[i]->GetIndexArrayCopy()));
	}

	const JSONElement& el = reader.GetElement("root");
	for (JSONElement::ConstMemberIterator it = el.MemberBegin(); it != el.MemberEnd(); it++)
	{
		std::string entityPath;
		entityPath = it->value["entity"].GetString();
		CU::Vector3f pos;
		reader._ReadElement(it->value["position"], pos);

		JSONReader entityReader(entityPath);

		Entity e = myEntityManager->CreateEntity();

		//All entities usually have a start position.
		myEntityManager->AddComponent<TranslationComponent>(e);
		TranslationComponent& t = myEntityManager->GetComponent<TranslationComponent>(e);
		t.myOrientation.SetPosition(pos);

		if (entityReader.HasElement("light"))
		{
			myEntityManager->AddComponent<LightComponent>(e);
			LightComponent& l = myEntityManager->GetComponent<LightComponent>(e);

			std::string type;
			entityReader.ReadElement("light", "type", type);
			if (type == "pointlight")
			{
				l.myType = eLightType::eSPOTLIGHT;
				reader._ReadElement(it->value["color"], l.color);
				l.color.r /= 255.f;
				l.color.g /= 255.f;
				l.color.b /= 255.f;
				reader.ReadElement(it->value["intensity"], l.intensity);
				reader.ReadElement(it->value["range"], l.range);
			}
		}

		if (entityReader.HasElement("render"))
		{
			myEntityManager->AddComponent<RenderComponent>(e);
			RenderComponent& r = myEntityManager->GetComponent<RenderComponent>(e);
			std::string entityModel[2];
			entityReader.ReadElement("render", entityModel);
			r.myModelID = myEngine->LoadModel(entityModel[0], entityModel[1]);
		}

		if (entityReader.HasElement("physics"))
		{
			const JSONElement& phys = entityReader.GetElement("physics");
			for (JSONElement::ConstMemberIterator obj = phys.MemberBegin(); obj != phys.MemberEnd(); obj++)
			{
				float mass = static_cast<float>(obj->value.GetDouble());
				myEntityManager->AddComponent<PhysicsComponent>(e);
				PhysicsComponent& p = myEntityManager->GetComponent<PhysicsComponent>(e);
				p.myBody = myPhysicsManager->CreateBody();
				myPhysicsManager->Add(p.myBody->InitAsSphere(1.f, mass, myPhysicsManager->GetGravityForce(), 1.293f, pos));
			}
		}
	}

	myEntityManager->AddSystem<CPhysicsSystem>(myPhysicsManager);
	myEntityManager->AddSystem<CRenderSystem>(mySynchronizer);
	myEntityManager->AddSystem<CLightSystem>(mySynchronizer);
}

CGame::~CGame()
{
	SAFE_DELETE(myPhysicsManager);
	SAFE_DELETE(myEntityManager);
	myTerrain.DeleteAll();
}

void CGame::Update(float aDeltaTime)
{
	myFrameCount++;
	myAverageFPS += myEngine->GetFPS();
	myTime -= aDeltaTime;
	if (myTime <= 0.f)
	{
		myFPSToPrint = static_cast<u32>(myAverageFPS / myFrameCount);
		myFrameCount = 0;
		myAverageFPS = 0.f;
		myTime = 1.f;
	}

	std::stringstream ss;
	const SLocalTime& locTime = myEngine->GetLocalTime();
	ss << myEngine->GetFPS() << "\n" << myFPSToPrint << "\nDeltaTime:" << aDeltaTime
		<< "\nLocal time : "
		<< locTime.hour << ":";

	if (locTime.minute < 10)
	{
		ss << "0" << locTime.minute;
	}
	else
		ss << locTime.minute << ":";

	if (locTime.second < 10)
	{
		ss << "0" << locTime.second;
	}
	else
		ss << locTime.second;

	mySynchronizer->AddRenderCommand(SRenderCommand(ss.str(), { 0, 0 }, eType::TEXT));
	myEntityManager->Update(aDeltaTime);




	mySynchronizer->AddRenderCommand(SRenderCommand(eType::TERRAIN));
}
