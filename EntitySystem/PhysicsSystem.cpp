#include "PhysicsSystem.h"
#include "TranslationComponent.h"
#include "PhysicsComponent.h"

#include "../Physics/RigidBody.h"
#include "../Physics/PhysicsManager.h"

CPhysicsSystem::CPhysicsSystem(CEntityManager& anEntityManager, CPhysicsManager* aPhysicsManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<STranslationComponent, SPhysicsComponent>>())
	, myPhysicsManager(aPhysicsManager)
{
}

void CPhysicsSystem::Update(float aDeltaTime)
{
	myAccumulatedTime += aDeltaTime;
	while (myAccumulatedTime >= 1.f / 60.f)
	{
		const CU::GrowingArray<Entity>& entities = GetEntities();
		for (int i = 0; i < entities.Size(); i++)
		{
			Entity e = entities[i];
			STranslationComponent& translation = GetComponent<STranslationComponent>(e);
			SPhysicsComponent& physics = GetComponent<SPhysicsComponent>(e);
			translation.myOrientation = physics.myBody->GetOrientation(physics.m_IsPlayer);
			physics.myBody->Update(aDeltaTime);

		}
		myPhysicsManager->Update(myAccumulatedTime); //ASync Physics?
		
		myAccumulatedTime -= 1.f / 60.f;
	}
}
