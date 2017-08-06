#include "PhysicsSystem.h"
#include "TranslationComponent.h"
#include "PhysicsComponent.h"
#include "DebugComponent.h"

#include <Physics/RigidBody.h>
#include <Physics/PhysicsManager.h>

#include <Engine/Engine.h>

PhysicsSystem::PhysicsSystem(NodeEntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<TranslationComponent, PhysicsComponent>>())
{
	myPhysicsManager = Engine::GetInstance()->GetPhysicsManager();
}

void PhysicsSystem::Update(float dt, bool paused)
{
	if (paused)
		return;

	const CU::GrowingArray<Entity>& entities = GetEntities();
	for (int i = 0; i < entities.Size(); i++)
	{
		Entity e = entities[i];
		TranslationComponent& translation = GetComponent<TranslationComponent>(e);

		PhysicsComponent& physics = GetComponent<PhysicsComponent>(e);


		translation.myOrientation = physics.myBody->GetOrientation();
		physics.myBody->Update(dt);


	}
}
