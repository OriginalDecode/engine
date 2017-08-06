#include "InputSystem.h"

#include "InputComponent.h"
#include "TranslationComponent.h"
#include "CameraComponent.h"

#include "../Input/ControllerInput.h"
#include "../Input/InputWrapper.h"
#include "../Input/InputHandle.h"
#include "../Engine/Camera.h"
#include "../Engine/Engine.h"
InputSystem::InputSystem(NodeEntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<CameraComponent, InputComponent>>())
{
}

void InputSystem::Update(float /*delta_time*/, bool paused)
{
	if (paused)
		return;
	//if (Engine::GetInstance()->IsDebugMenuActive())
	//	return;

	const CU::GrowingArray<Entity>& entities = GetEntities();
	for (const Entity& e : entities)
	{

		InputComponent& input = GetComponent<InputComponent>(e);
		input.m_InputHandle->HandleInput();

	}
}

