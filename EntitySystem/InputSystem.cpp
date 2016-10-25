#include "InputSystem.h"

#include "InputComponent.h"
#include "TranslationComponent.h"
#include "PhysicsComponent.h"
#include "../Physics/RigidBody.h"
#include "../Physics/PhysicsManager.h"
#include "../Input/InputWrapper.h"
#include "../Input/InputHandle.h"

InputSystem::InputSystem(CEntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<PhysicsComponent, InputController>>())
{
}

void InputSystem::Update(float delta_time)
{
	const CU::GrowingArray<Entity>& entities = GetEntities();
	for (const Entity& e : entities)
	{
		InputController& input = GetComponent<InputController>(e);
		input.m_InputHandle->HandleInput();
		input.m_InputHandle->Update();
	}
}

