#include "RenderSystem.h"
#include <sstream>
#include "TranslationComponent.h"
#include "RenderComponent.h"
#include "LightComponent.h"

/* Engine Includes */
#include "Synchronizer.h"
#include "RenderCommand.h"

CRenderSystem::CRenderSystem(CEntityManager& anEntityManager, Snowblind::CSynchronizer* aSynchronizer)
	: CBaseSystem(anEntityManager, CreateFilter<Requires<STranslationComponent, SRenderComponent>>())
	, mySynchronizer(aSynchronizer)
{
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

		mySynchronizer->AddRenderCommand(SRenderCommand(eType::MODEL, render.myModelID, translation.myOrientation.GetPosition() ));
	}
}
