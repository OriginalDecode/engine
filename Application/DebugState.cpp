#include "DebugState.h"
#include <EventManager.h>

#include "../Input/InputHandle.h"
#include <Engine.h>
#include <StateStack.h>
void DebugState::InitState(StateStack* statestack)
{
    m_StateStack = statestack;
    EventManager::Send("Debug_State_Active", nullptr);
}

void DebugState::EndState()
{
    EventManager::Send("Debug_State_Deactive", nullptr);
}

void DebugState::Update(float dt)
{
    //InputWrapper* input_wrapper = Engine::GetInstance()->GetInputHandle()->GetInputWrapper();
    //if(input_wrapper->IsDown(KButton::LSHIFT) && input_wrapper->OnDown(KButton::D))
    //{
    //    m_StateStack->PopCurrentSubState();
    //}
}

void DebugState::Render(bool render_through)
{

}
