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

	//Save chunks with heightmap data embedded. They should all have a WIDTH x HEIGHT amount of data in the height map that is saved in the map data.

	JSONReader reader("Data/Levels/level_01.json");
	myTerrain = myEngine->CreateTerrain(512, 512);

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
				l.myType = eLightType::ePOINTLIGHT;
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
				p.myBody = myPhysicsManager->CreateBody(mass);
				myPhysicsManager->Add(p.myBody->InitAsSphere(pos));
			}
		}
	}

	myEntityManager->AddSystem<CPhysicsSystem>();
	myEntityManager->AddSystem<CRenderSystem>(mySynchronizer);
	myEntityManager->AddSystem<CLightSystem>(mySynchronizer);
}

CGame::~CGame()
{
	SAFE_DELETE(myPhysicsManager);
	SAFE_DELETE(myEntityManager);
	SAFE_DELETE(myTerrain);
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
		<< locTime.hour << " : ";

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

	mySynchronizer->AddRenderCommand(SRenderCommand(ss.str(), { 0,0 }, SRenderCommand::eType::TEXT));
	myEntityManager->Update(aDeltaTime);


	myAccumulatedTime += aDeltaTime;
	myPhysicsManager->Update(myAccumulatedTime);


	mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::TERRAIN));
}
