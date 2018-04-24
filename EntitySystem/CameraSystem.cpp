#include "CameraSystem.h"

#include "TranslationComponent.h"
#include "CameraComponent.h"
#include <Camera.h>
CameraSystem::CameraSystem(_EntityManager& entitymanager)
	: BaseSystem(entitymanager, CreateFilter<Requires<TranslationComponent, CameraComponent>>())
{
}

void CameraSystem::Update(float /*delta_time*/, bool paused)
{
	if (paused)
		return;
	//const CU::GrowingArray<Entity>& entities = GetEntities();
	//for (const Entity& e : entities)
	//{
	//	//STranslationComponent& translation = GetComponent<STranslationComponent>(e);
	//	//CameraComponent& camera = GetComponent<CameraComponent>(e);
	///*	if(!camera.m_Camera->GetCanFreeFly())
	//		camera.m_Camera->SetOrientation(translation.myOrientation);*/
	//}
}
