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


		CU::Matrix44f copy = translation.myOrientation;
		

		translation.myOrientation = physics.m_Body->GetOrientation();
		CU::Vector3f force = CU::Math::GetNormalized(translation.myOrientation.GetPosition() - copy.GetPosition());
		force = { 1.f,0.f,1.f };
		//if(CU::Math::Length(force) > 0.f)

		physics.m_Body->Update(dt);
			physics.m_Body->Impulse(force*10.f);


	}
}
