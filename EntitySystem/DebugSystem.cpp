#include "DebugSystem.h"
#include "Engine.h"
#include "DebugComponent.h"
#include "TranslationComponent.h"
DebugSystem::DebugSystem(EntityManager& entity_manager)
	: BaseSystem(entity_manager, CreateFilter<Requires<TranslationComponent, DebugComponent>>())
{
	m_PhysicsManager = Hex::Engine::GetInstance()->GetPhysicsManager();
}
#define FIXED_TIMESTEP 1.f / 60.f
void DebugSystem::Update(float dt)
{
		const CU::GrowingArray<Entity>& entities = GetEntities();
		for (Entity e : entities)
		{
			DebugComponent& debug = GetComponent<DebugComponent>(e);
			TranslationComponent& translation = GetComponent<TranslationComponent>(e);


		}
}

