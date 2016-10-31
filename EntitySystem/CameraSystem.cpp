#include "CameraSystem.h"

#include "TranslationComponent.h"
#include "CameraComponent.h"
#include <Camera.h>
CameraSystem::CameraSystem(CEntityManager& entitymanager)
	: BaseSystem(entitymanager, CreateFilter<Requires<STranslationComponent, CameraComponent>>())
{
}

void CameraSystem::Update(float delta_time)
{
	const CU::GrowingArray<Entity>& entities = GetEntities();
	for (const Entity& e : entities)
	{
		STranslationComponent& translation = GetComponent<STranslationComponent>(e);
		CameraComponent& camera = GetComponent<CameraComponent>(e);
		camera.m_Camera->SetOrientation(translation.myOrientation);
	}
}
