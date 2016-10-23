#include "InputSystem.h"

#include "InputComponent.h"
#include "TranslationComponent.h"
#include "PhysicsComponent.h"
#include "../Physics/RigidBody.h"
#include "../Physics/PhysicsManager.h"
#include "../Input/InputWrapper.h"
InputSystem::InputSystem(CEntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<PhysicsComponent, InputController>>())
{
}

void InputSystem::Update(float delta_time)
{
	const CU::GrowingArray<Entity>& entities = GetEntities();
	for (const Entity& e : entities)
	{
		PhysicsComponent& physics = GetComponent<PhysicsComponent>(e);
		if(CU::Input::InputWrapper::GetInstance()->KeyDown(W))
		{
			physics.myBody->Impulse(CU::Vector3f(0.f, 0.f, 1500.f));
		}

		if (CU::Input::InputWrapper::GetInstance()->KeyDown(S))
		{
			physics.myBody->Impulse(CU::Vector3f(0.f, 0.f, -1500.f));
		}

		if (CU::Input::InputWrapper::GetInstance()->KeyDown(A))
		{
			physics.myBody->Impulse(CU::Vector3f(-1500.f, 0.f, 0.f));
		}

		if (CU::Input::InputWrapper::GetInstance()->KeyDown(D))
		{
			physics.myBody->Impulse(CU::Vector3f(1500.f, 0.f, 0.f));
		}

		if (CU::Input::InputWrapper::GetInstance()->KeyDown(SPACE))
		{
			physics.myBody->Impulse(CU::Vector3f(0.f, 100.f, 0.f));
		}
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(X))
		{
			physics.myBody->Impulse(CU::Vector3f(0.f, -100.f, 0.f));
		}
	}
}
