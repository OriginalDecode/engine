#include "InputSystem.h"

#include "InputComponent.h"
#include "TranslationComponent.h"
#include "CameraComponent.h"

#include "../Input/ControllerInput.h"
#include "../Input/InputWrapper.h"
#include "../Input/InputHandle.h"
#include "../Engine/Camera.h"
#include "../Engine/Engine.h"
InputSystem::InputSystem(CEntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<CameraComponent, InputController>>())
{
}

void InputSystem::Update(float delta_time)
{
	if (Snowblind::CEngine::GetInstance()->IsDebugMenuActive())
		return;

	const CU::GrowingArray<Entity>& entities = GetEntities();
	for (const Entity& e : entities)
	{

		InputController& input = GetComponent<InputController>(e);
		input.m_InputHandle->HandleInput();

	}
}

