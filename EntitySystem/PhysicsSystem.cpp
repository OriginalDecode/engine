#include "PhysicsSystem.h"
#include "TranslationComponent.h"
#include "PhysicsComponent.h"

#include "../Physics/RigidBody.h"
#include "../Physics/PhysicsManager.h"
#include "AABBComponent.h"

#include "../Engine/Engine.h"

CPhysicsSystem::CPhysicsSystem(EntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<STranslationComponent, SPhysicsComponent>>())
{
	myPhysicsManager = Snowblind::Engine::GetInstance()->GetPhysicsManager();
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
			if (!physics.myBody->IsEnabled())
				continue;

			translation.myOrientation = physics.myBody->GetOrientation();
			physics.myBody->Update(aDeltaTime);

			if(!physics.m_IsPlayer)
			{
				AABBComponent& aabb = GetComponent<AABBComponent>(e);
				if (aabb.m_Body)
					aabb.m_Body->Update(aDeltaTime);
			}

		}
		myPhysicsManager->Update(myAccumulatedTime); //ASync Physics?
		myAccumulatedTime -= 1.f / 60.f;
	}
}
