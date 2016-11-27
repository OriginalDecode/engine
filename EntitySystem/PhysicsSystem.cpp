#include "PhysicsSystem.h"
#include "TranslationComponent.h"
#include "PhysicsComponent.h"

#include "../Physics/RigidBody.h"
#include "../Physics/PhysicsManager.h"
#include "AABBComponent.h"
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
			translation.myOrientation = physics.myBody->GetOrientation();
			physics.myBody->Update(aDeltaTime);

			AABBComponent& aabb = GetComponent<AABBComponent>(e);
			if(aabb.m_Body)
			aabb.m_Body->Update(aDeltaTime);



		}
		myPhysicsManager->Update(myAccumulatedTime); //ASync Physics?
		
		myAccumulatedTime -= 1.f / 60.f;
	}
}
