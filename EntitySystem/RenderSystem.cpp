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

CRenderSystem::CRenderSystem(CEntityManager& anEntityManager, Snowblind::CSynchronizer* aSynchronizer)
	: BaseSystem(anEntityManager, CreateFilter<Requires<STranslationComponent, SRenderComponent, AABBComponent>>())
	, mySynchronizer(aSynchronizer)
{
}

void CRenderSystem::Update(float /*aDeltaTime*/)
{
	const CU::GrowingArray<Entity>& entities = GetEntities();

	/* Add more commands? */
	for (int i = 0; i < entities.Size(); i++)
	{
		
			Entity e = entities[i];
			TranslationComponent& translation = GetComponent<TranslationComponent>(e);

			translation.myOrientation = CU::Matrix44f::CreateRotateAroundY(0.2f * 0.016f) * translation.myOrientation;

			RenderComponent& render = GetComponent<RenderComponent>(e);
			AABBComponent& aabb = GetComponent<AABBComponent>(e);

			aabb.m_AABB.Update(Snowblind::CEngine::GetInstance()->GetModel(render.myModelID)->GetOrientation());
			const CU::GrowingArray<RenderCommand> commands = aabb.m_AABB.GetCommands();
			for (const RenderCommand& command : commands)
			{
				mySynchronizer->AddRenderCommand(command);
			}

			mySynchronizer->AddRenderCommand(RenderCommand(eType::MODEL, render.myModelID, translation.myOrientation));

	}
}
