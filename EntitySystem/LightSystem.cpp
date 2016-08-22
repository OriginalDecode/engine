#include "LightSystem.h"
#include <assert.h>
#include "TranslationComponent.h"
#include "LightComponent.h"

/* Engine Includes */
#include "Synchronizer.h"
#include "RenderCommand.h"

CLightSystem::CLightSystem(CEntityManager& anEntityManager, Snowblind::CSynchronizer* aSynchronizer)
	: CBaseSystem(anEntityManager, CreateFilter<Requires<STranslationComponent, LightComponent>>())
	, mySynchronizer(aSynchronizer)
{
}

CLightSystem::~CLightSystem()
{
}

void CLightSystem::Update(float aDeltaTime)
{
	const CU::GrowingArray<Entity>& entities = GetEntities();

	/* Add more commands? */
	for (int i = 0; i < entities.Size(); i++)
	{
		Entity e = entities[i];
		TranslationComponent& translation = GetComponent<TranslationComponent>(e);
		LightComponent& light = GetComponent<LightComponent>(e);

		if (light.myType == eLightType::ePOINTLIGHT)
			mySynchronizer->AddRenderCommand(SRenderCommand(eType::POINTLIGHT, translation.myOrientation.GetPosition(), light.color, light.intensity, light.range));

		if (light.myType == eLightType::eSPOTLIGHT)
			mySynchronizer->AddRenderCommand(SRenderCommand(eType::SPOTLIGHT, translation.myOrientation.GetPosition(), light.color, light.intensity, light.range));

	}
}
