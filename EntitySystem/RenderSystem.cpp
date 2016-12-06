#include "RenderSystem.h"

#include <Engine.h>
#include "../Engine/Model.h"
#include "TranslationComponent.h"
#include "RenderComponent.h"
#include "LightComponent.h"

/* Engine Includes */
#include "Synchronizer.h"
#include "RenderCommand.h"
#include "AABBComponent.h"
#include <Utilities.h>

#include "../Engine/Engine.h"

CRenderSystem::CRenderSystem(EntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<STranslationComponent, SRenderComponent>>())
{
	mySynchronizer = Snowblind::Engine::GetInstance()->GetSynchronizer();
}

void CRenderSystem::Update(float /*aDeltaTime*/)
{
	const CU::GrowingArray<Entity>& entities = GetEntities();

	for (int i = 0; i < entities.Size(); i++)
	{
			Entity e = entities[i];
			TranslationComponent& translation = GetComponent<TranslationComponent>(e);

			RenderComponent& render = GetComponent<RenderComponent>(e);
			mySynchronizer->AddRenderCommand(RenderCommand(eType::MODEL, render.myModelID, translation.myOrientation));
	}
}
