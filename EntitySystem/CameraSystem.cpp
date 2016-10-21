#include "CameraSystem.h"

#include "TranslationComponent.h"
#include "CameraComponent.h"

CameraSystem::CameraSystem(CEntityManager& entitymanager)
	: BaseSystem(entitymanager, CreateFilter<Requires<STranslationComponent, CameraComponent>>())
{
}

void CameraSystem::Update(float delta_time)
{

}
