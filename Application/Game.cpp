#include "Game.h"

/* Entity System Includes */
#include <EntityManager.h>
/* Components */
#include <TranslationComponent.h>
#include <RenderComponent.h>
/* Systems */
#include <RenderSystem.h>
/* End of Entity System Includes*/
#include <AssetsContainer.h>
#include <EngineDefines.h>
#include <JSON/JSONReader.h>


struct SGameObject
{
	std::string entityPath;
	CU::Vector3f pos;
};


CGame::CGame(Snowblind::CSynchronizer* aSynchronizer)
	: mySynchronizer(aSynchronizer)
{
	myEntityManager = new CEntityManager();
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
			r.myModelID = Snowblind::CAssetsContainer::GetInstance()->LoadModel(entityModel[0], entityModel[1]);
		}
	}

	myEntityManager->AddSystem<CRenderSystem>(mySynchronizer);

}

CGame::~CGame()
{
	SAFE_DELETE(myEntityManager);
}

void CGame::Update(float aDeltaTime)
{
	myEntityManager->Update(aDeltaTime);
}
