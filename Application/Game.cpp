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

CGame::CGame(Snowblind::CSynchronizer* aSynchronizer)
	: mySynchronizer(aSynchronizer)
{
	myEntityManager = new CEntityManager();

	JSONReader reader("Data/Levels/level_01.json");
	std::string entityPath;
	reader.ReadElement("object", "entity", entityPath);
	CU::Vector3f pos;
	reader.ReadElement("object", "position", pos);

	JSONReader entityReader(entityPath);
	bool hasTranslation;
	entityReader.ReadElement("components", "Translation", hasTranslation);
	std::string entityModel;
	entityReader.ReadElement("components", "Render", entityModel);

	Entity e = myEntityManager->CreateEntity();
	if (hasTranslation)
	{
		myEntityManager->AddComponent<TranslationComponent>(e);
		TranslationComponent& t = myEntityManager->GetComponent<TranslationComponent>(e);
		t.myOrientation.SetPosition(pos);
	}

	if (entityModel != "")
	{
		myEntityManager->AddComponent<RenderComponent>(e);
		RenderComponent& r = myEntityManager->GetComponent<RenderComponent>(e);
		r.myModelID = Snowblind::CAssetsContainer::GetInstance()->LoadModel(entityModel, "Data/Shaders/T_Deferred_Base.json");
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
