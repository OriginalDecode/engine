#include "PhysicsSystem.h"
#include "TranslationComponent.h"
#include "PhysicsComponent.h"
#include "DebugComponent.h"

#include "../Physics/RigidBody.h"
#include "../Physics/PhysicsManager.h"
#include "AABBComponent.h"

#include "../Engine/Engine.h"
#include "../Engine/Synchronizer.h"
#include "../Engine/RenderCommand.h"

PhysicsSystem::PhysicsSystem(NodeEntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<TranslationComponent, PhysicsComponent>>())
{
	myPhysicsManager = Engine::GetInstance()->GetPhysicsManager();
}

void PhysicsSystem::Update(float aDeltaTime)
{
	myAccumulatedTime += aDeltaTime;
	while (myAccumulatedTime >= 1.f / 60.f)
	{
		const CU::GrowingArray<Entity>& entities = GetEntities();
		for (int i = 0; i < entities.Size(); i++)
		{
			Entity e = entities[i];
			TranslationComponent& translation = GetComponent<TranslationComponent>(e);
			
			PhysicsComponent& physics = GetComponent<SPhysicsComponent>(e);
			

			translation.myOrientation = physics.myBody->GetOrientation();
			physics.myBody->Update(aDeltaTime);
			
			
		}
		//myPhysicsManager->Update(myAccumulatedTime); //ASync Physics? needs to be moved into separete sim thread or something
		myAccumulatedTime -= 1.f / 60.f;
	}
}
