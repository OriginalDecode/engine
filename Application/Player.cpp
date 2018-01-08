#include "Player.h"

#include <Physics/RigidBody.h>
#include <Physics/PhysicsManager.h>

#include <Engine/Engine.h>

#include <EntitySystem/EntityManager.h>

#include <Input/InputHandle.h>
#include <Input/ControllerInput.h>


Player::~Player()
{
	delete m_Body;
}

TreeDweller* Player::Initiate()
{
	Engine* engine = Engine::GetInstance();

	//m_Body = new RigidBody;
	//m_Body->Capsule({ 0.f,0.f,0.f });
	//engine->GetPhysicsManager()->Add(m_Body->GetBody());

	EntityManager& em = engine->GetEntityManager();

	TreeDweller* dweller = new TreeDweller;
	m_EntityID = em.CreateEntity();
	dweller->AddComponent(&em.AddComponent<TranslationComponent>(m_EntityID), TreeDweller::TRANSLATION);
	dweller->AddComponent(&em.AddComponent<GraphicsComponent>(m_EntityID), TreeDweller::GRAPHICS);
	//dweller->AddComponent(&em.AddComponent<PhysicsComponent>(m_EntityID), TreeDweller::PHYSICS);

	return dweller;
}

void Player::Update(float dt)
{
	InputHandle* input = Engine::GetInstance()->GetInputHandle();


	/* MECHANICS */

	/* 
		Do the input things?
	*/


	ControllerInput* control = input->GetController(0);
	if (control)
	{
		const ControllerState& state = control->GetState();
		const ControllerState& prev_state = control->GetState();
		if (control->OnDown(eA))
		{
			int apa;
			apa = 5;
		}
	}
}
