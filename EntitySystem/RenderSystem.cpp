#include "RenderSystem.h"

#include "TranslationComponent.h"
#include "RenderComponent.h"

/* Engine Includes */
#include "Synchronizer.h"
#include "RenderCommand.h"

CRenderSystem::CRenderSystem(CEntityManager& anEntityManager, Snowblind::CSynchronizer* aSynchronizer)
	: CBaseSystem(anEntityManager, CreateFilter<Requires<STranslationComponent, SRenderComponent>>())
{
	mySynchronizer = aSynchronizer;
	int apa;
	apa = 5;
}

CRenderSystem::~CRenderSystem()
{
}

void CRenderSystem::Update(float aDeltaTime)
{
	const CU::GrowingArray<Entity>& entities = GetEntities();

	/* Add more commands? */
	for (int i = 0; i < entities.Size(); i++)
	{
		Entity e = entities[i];
		TranslationComponent& translation = GetComponent<TranslationComponent>(e);
		RenderComponent& render = GetComponent<RenderComponent>(e);

		mySynchronizer->AddRenderCommand(SRenderCommand(SRenderCommand::eType::MODEL, render.myModelID, translation.myOrientation.GetPosition() ));
	}
}
