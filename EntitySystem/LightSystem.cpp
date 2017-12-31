#include "LightSystem.h"
#include "TranslationComponent.h"
#include "LightComponent.h"

/* Engine Includes */
#include "Synchronizer.h"
#include "RenderCommand.h"
#include "../Engine/Engine.h"
LightSystem::LightSystem(NodeEntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<TranslationComponent, LightComponent>>())
{
	mySynchronizer = Engine::GetInstance()->GetSynchronizer();
}

void LightSystem::Update(float /*aDeltaTime*/, bool paused)
{
	const CU::GrowingArray<Entity>& entities = GetEntities();

	/* Add more commands? */
	for (int i = 0; i < entities.Size(); i++)
	{
		Entity e = entities[i];
		TranslationComponent& translation = GetComponent<TranslationComponent>(e);
		LightComponent& light = GetComponent<LightComponent>(e);
		

		if (light.myType == eLightType::ePOINTLIGHT)
			mySynchronizer->AddRenderCommand(PointlightCommand(0, light.range, light.intensity, light.color, translation.m_Orientation));

		if ( light.myType == eLightType::eSPOTLIGHT )
			mySynchronizer->AddRenderCommand(SpotlightCommand(light.m_LightID, light.angle, light.range, light.intensity, light.color, translation.m_Orientation, false));
			//mySynchronizer->AddRenderCommand(RenderCommand(eType::SPOTLIGHT, translation.myOrientation.GetPosition(), light.color, light.angle, light.range, light.direction, translation.myOrientation, light.m_LightID, true));

	}
}
