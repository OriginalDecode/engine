#include "LightSystem.h"
#include <assert.h>
#include "TranslationComponent.h"
#include "LightComponent.h"

/* Engine Includes */
#include "Synchronizer.h"
#include "RenderCommand.h"
#include "../Engine/Engine.h"
LightSystem::LightSystem(EntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<STranslationComponent, LightComponent>>())
{
	mySynchronizer = Engine::GetInstance()->GetSynchronizer();
}

void LightSystem::Update(float /*aDeltaTime*/)
{
	const CU::GrowingArray<Entity>& entities = GetEntities();

	/* Add more commands? */
	for (int i = 0; i < entities.Size(); i++)
	{
		Entity e = entities[i];
		TranslationComponent& translation = GetComponent<TranslationComponent>(e);
		LightComponent& light = GetComponent<LightComponent>(e);

		if (light.myType == eLightType::ePOINTLIGHT)
			mySynchronizer->AddRenderCommand(RenderCommand(eType::POINTLIGHT, translation.myOrientation.GetPosition(), light.color, light.intensity, light.range, false));

		if (light.myType == eLightType::eSPOTLIGHT)
			mySynchronizer->AddRenderCommand(RenderCommand(eType::SPOTLIGHT, translation.myOrientation.GetPosition(), light.color, light.angle, light.range, light.direction, translation.myOrientation, light.m_LightID, true));

	}
}
