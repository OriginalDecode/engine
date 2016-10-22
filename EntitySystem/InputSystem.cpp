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
	const float move_speed = 15.f;
	for (const Entity& e : entities)
	{
		PhysicsComponent& physics = GetComponent<PhysicsComponent>(e);
		//position = physics.myBody->GetOrientation().GetPosition();//physics.myOrientation.GetTranslation();

		float multiplier = 1.f;
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(LSHIFT))
		{
			multiplier = 100.f;
		}
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
		/*if (CU::Input::InputWrapper::GetInstance()->KeyDown(W))
		{
			CU::Vector4f forward = translation.myOrientation.GetForward();
			position += forward * (move_speed * delta_time) * multiplier;
		}
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(S))
		{
			CU::Vector4f forward = translation.myOrientation.GetForward();
			position += forward * (-move_speed * delta_time) * multiplier;
		}

		if (CU::Input::InputWrapper::GetInstance()->KeyDown(SPACE))
		{
			CU::Vector4f up = translation.myOrientation.GetUp();
			position += up * (move_speed * delta_time) * multiplier;
		}
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(X))
		{
			CU::Vector4f up = translation.myOrientation.GetUp();
			position += up * (-move_speed * delta_time) * multiplier;
		}

		if (CU::Input::InputWrapper::GetInstance()->KeyDown(D))
		{
			CU::Vector4f right = translation.myOrientation.GetRight();
			position += right * (move_speed * delta_time) * multiplier;
		}
		if (CU::Input::InputWrapper::GetInstance()->KeyDown(A))
		{
			CU::Vector4f right = translation.myOrientation.GetRight();
			position += right * (-move_speed * delta_time) * multiplier;
		}*/

	}
}
