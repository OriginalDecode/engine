#include "PhysicsSystem.h"
#include "TranslationComponent.h"
#include "PhysicsComponent.h"
#include "DebugComponent.h"

#include <Physics/RigidBody.h>
#include <Physics/PhysicsManager.h>

#include <Engine/Engine.h>

#include <Engine/DebugHandle.h>

PhysicsSystem::PhysicsSystem(_EntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<TranslationComponent, PhysicsComponent>>())
{
	myPhysicsManager = Engine::GetInstance()->GetPhysicsManager();
}

void PhysicsSystem::Update(float dt, bool paused)
{
#ifdef _DEBUG
	if (debug::DebugHandle::s_PausePhysics)
		return;
#endif
	const CU::GrowingArray<Entity>& entities = GetEntities();

	for (Entity e : entities)
	{
		TranslationComponent& translation = GetComponent<TranslationComponent>(e);
		if (translation.m_UpdatedOnNetwork)
		{
			translation.m_UpdatedOnNetwork = false;
			continue;
		}

		PhysicsComponent& physics = GetComponent<PhysicsComponent>(e);
		translation.SetOrientation(physics.m_Body->GetOrientation());
		physics.m_Body->Update(dt);

	}
}
