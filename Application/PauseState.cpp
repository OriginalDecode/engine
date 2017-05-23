#include "PauseState.h"
#include <Engine.h>
#include <Synchronizer.h>
#include <RenderCommand.h>
#include "../Input/InputHandle.h"
#include "StateStack.h"
void CPauseState::InitState(StateStack* state_stack)
{
	m_StateStack = state_stack;
}

void CPauseState::Update(float aDeltaTime)
{
	Engine::GetInstance()->GetSynchronizer()->AddRenderCommand(TextCommand("Hello World!", CU::Vector2f(0.5f, 0.5f)), eBufferType::TEXT_BUFFER);
	InputWrapper* input_wrapper = Engine::GetInstance()->GetInputHandle()->GetInputWrapper();
	if (input_wrapper->OnDown(KButton::ESCAPE))
	{
		m_StateStack->PopCurrentSubState();
	}
}

void CPauseState::EndState()
{

}

void CPauseState::Render(bool render_through)
{

}
