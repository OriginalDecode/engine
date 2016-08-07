#include "PhysicsSystem.h"
#include "TranslationComponent.h"
#include "PhysicsComponent.h"
#include "../Physics/RigidBody.h"
CPhysicsSystem::CPhysicsSystem(CEntityManager& anEntityManager)
	: CBaseSystem(anEntityManager, CreateFilter<Requires<STranslationComponent, SPhysicsComponent>>())
{
}

CPhysicsSystem::~CPhysicsSystem()
{
}

void CPhysicsSystem::Update(float aDeltaTime)
{
	const CU::GrowingArray<Entity>& entities = GetEntities();
	for (int i = 0; i < entities.Size(); i++)
	{
		Entity e = entities[i];
		STranslationComponent& translation = GetComponent<STranslationComponent>(e);
		SPhysicsComponent& physics = GetComponent<SPhysicsComponent>(e);

		translation.myOrientation = physics.myBody->GetOrientation();
		physics.myBody->Impulse(CU::Vector3f(0, -500, 0));
	}
}
