#include "Game.h"

/* Entity System Includes */
#include <EntityManager.h>
/* Components */
#include <PhysicsComponent.h>
#include <RenderComponent.h>
#include <TranslationComponent.h>
/* Systems */
#include <PhysicsSystem.h>
#include <RenderSystem.h>
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

struct SGameObject
{
	std::string entityPath;
	CU::Vector3f pos;
};


CGame::CGame(Snowblind::CSynchronizer* aSynchronizer)
	: mySynchronizer(aSynchronizer)
{
	myEntityManager = new CEntityManager();
	myPhysicsManager = new CPhysicsManager();
	myEngine = Snowblind::CEngine::GetInstance();

	myEngine->ToggleVsync();
	JSONReader reader("Data/Levels/level_01.json");

	/*
		Somewhere in the itteration we will look for a terrain file or something
	*/
#if defined (_DEBUG)
	//HRESULT hr = URLDownloadToFile(NULL, ("https://bitbucket.org/api/1.0/repositories/originaldecode/snowblind-engine/changesets?limit=0"), ("snowblind_data.json"), 0, 0);
#endif
	myTerrain = myEngine->CreateTerrain(1000, 1000);

	const JSONElement& el = reader.GetElement("root");
	for (JSONElement::ConstMemberIterator it = el.MemberBegin(); it != el.MemberEnd(); it++)
	{
		std::string entityPath;
		entityPath = it->value["entity"].GetString();
		CU::Vector3f pos;
		reader._ReadElement(it->value["position"], pos);

		JSONReader entityReader(entityPath);
		bool hasTranslation = false;
		entityReader.ReadElement("Translation", hasTranslation);
		std::string entityModel[2];
		entityReader.ReadElement("Render", entityModel);

		Entity e = myEntityManager->CreateEntity();
		if (hasTranslation)
		{
			myEntityManager->AddComponent<TranslationComponent>(e);
			TranslationComponent& t = myEntityManager->GetComponent<TranslationComponent>(e);
			t.myOrientation.SetPosition(pos);
		}

		if (entityModel[0] != "")
		{
			myEntityManager->AddComponent<RenderComponent>(e);
			RenderComponent& r = myEntityManager->GetComponent<RenderComponent>(e);
			r.myModelID = myEngine->LoadModel(entityModel[0], entityModel[1]);
		}

		bool hasPhysics = false;
		entityReader.ReadElement("Physics", hasPhysics);

		if (hasPhysics)
		{
			myEntityManager->AddComponent<PhysicsComponent>(e);
			PhysicsComponent& p = myEntityManager->GetComponent<PhysicsComponent>(e);
			p.myBody = myPhysicsManager->CreateBody(10);//new CRigidBody();
			myPhysicsManager->Add(p.myBody->InitAsSphere(pos));

		}
	}

	myEntityManager->AddSystem<CPhysicsSystem>();
	myEntityManager->AddSystem<CRenderSystem>(mySynchronizer);

}

CGame::~CGame()
{
	SAFE_DELETE(myPhysicsManager);
	SAFE_DELETE(myEntityManager);
	SAFE_DELETE(myTerrain);
}

void CGame::Update(float aDeltaTime)
{
	std::stringstream ss;
	const SLocalTime& locTime = myEngine->GetLocalTime();
	ss << myEngine->GetFPS() << "\nDeltaTime:" << aDeltaTime
		<< "\nLocal time : "
		<< locTime.hour << ":"
		<< locTime.minute << ":";
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
