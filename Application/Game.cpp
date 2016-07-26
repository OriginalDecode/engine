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


	JSONReader reader("Data/Levels/level_01.json");

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
			r.myModelID = Snowblind::CEngine::GetInstance()->GetAssetsContainer()->LoadModel(entityModel[0], entityModel[1]);
		}

		bool hasPhysics = false;
		entityReader.ReadElement("Physics", hasPhysics);

		if (hasPhysics)
		{
			myEntityManager->AddComponent<PhysicsComponent>(e);
			PhysicsComponent& p = myEntityManager->GetComponent<PhysicsComponent>(e);
#ifdef _DEBUG
			p.myBody = new CRigidBody(mySynchronizer);
#else
			p.myBody = new CRigidBody();
#endif
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
}

void CGame::Update(float aDeltaTime)
{
	myPhysicsManager->Update();
	myEntityManager->Update(aDeltaTime);
	
}
