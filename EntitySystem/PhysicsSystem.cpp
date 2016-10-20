#include "PhysicsSystem.h"
#include "TranslationComponent.h"
#include "PhysicsComponent.h"

#include "../Physics/RigidBody.h"
#include "../Physics/PhysicsManager.h"

CPhysicsSystem::CPhysicsSystem(CEntityManager& anEntityManager, CPhysicsManager* aPhysicsManager)
	: CBaseSystem(anEntityManager, CreateFilter<Requires<STranslationComponent, SPhysicsComponent>>())
	, myPhysicsManager(aPhysicsManager)
{
}

CPhysicsSystem::~CPhysicsSystem()
{
}

void CPhysicsSystem::Update(float aDeltaTime)
{
	myAccumulatedTime += aDeltaTime;
	while (myAccumulatedTime >= 1.f / 60.f)
	{
		myPhysicsManager->Update(myAccumulatedTime); //ASync Physics?
		const CU::GrowingArray<Entity>& entities = GetEntities();
		for (int i = 0; i < entities.Size(); i++)
		{
			Entity e = entities[i];
			STranslationComponent& translation = GetComponent<STranslationComponent>(e);
			SPhysicsComponent& physics = GetComponent<SPhysicsComponent>(e);

			translation.myOrientation = physics.myBody->GetOrientation();
			physics.myBody->Update(aDeltaTime);

		}
		
		myAccumulatedTime -= 1.f / 60.f;
	}
}
