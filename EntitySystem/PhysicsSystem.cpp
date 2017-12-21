#include "PhysicsSystem.h"
#include "TranslationComponent.h"
#include "PhysicsComponent.h"
#include "DebugComponent.h"

#include <Physics/RigidBody.h>
#include <Physics/PhysicsManager.h>

#include <Engine/Engine.h>

#include <Engine/DebugHandle.h>

PhysicsSystem::PhysicsSystem(NodeEntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<TranslationComponent, PhysicsComponent>>())
{
	myPhysicsManager = Engine::GetInstance()->GetPhysicsManager();
}

void PhysicsSystem::Update(float dt, bool paused)
{
	if (debug::DebugHandle::s_PausePhysics)
		return;

	const CU::GrowingArray<Entity>& entities = GetEntities();

	for (Entity e : entities)
	{
		TranslationComponent& translation = GetComponent<TranslationComponent>(e);
		PhysicsComponent& physics = GetComponent<PhysicsComponent>(e);

		translation.SetOrientation(physics.m_Body->GetOrientation());

		physics.m_Body->Impulse(CU::Vector3f(0,0,0.5));
		physics.m_Body->Update(dt);

	}
}
