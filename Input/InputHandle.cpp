#include "InputHandle.h"
#include <hashlist.h>
#include "../Engine/Engine.h"

#include "ControllerInput.h"
#include "InputWrapper.h"

bool InputHandle::Initiate(HWND window_handle, HINSTANCE window_instance)
{

	Engine* engine = Engine::GetInstance();
	if (!engine)
		return false;

	m_Input = new InputWrapper;
	if (!m_Input)
		return false;

	if (!m_Input->Initiate(window_handle, window_instance))
		return false;
	
	return true;
}

void InputHandle::AddController(u16 controller_id)
{
	DL_ASSERT_EXP(m_ControllerID < m_ControllerMaxAmount, "Can't add more than %d controllers", m_ControllerMaxAmount);
	m_Controller[m_ControllerID++] = new ControllerInput(controller_id);
	DL_ASSERT_EXP(m_Controller[controller_id], "Failed to create a controller!");
}

void InputHandle::CleanUp()
{
	for (ControllerInput* c : m_Controller)
	{
		delete c;
		c = nullptr;
	}

	delete m_Input;
	m_Input = nullptr;
}

void InputHandle::Update(float dt)
{
	if (!Engine::GetInstance()->IsWindowActive())
		return;
	
	for (s32 i = 0; i < m_ControllerID; i++)
	{
		m_Controller[i]->Update();
	}
	
	m_Input->Update();
}

void InputHandle::HandleInput()
{
	
}

void InputHandle::Bind(u32 hash, std::function<void()> function)
{
	m_Functions.emplace(hash, function);
}

float InputHandle::GetX()
{
//	return m_Input->GetX();
	return 0.f;
}

float InputHandle::GetY()
{
	//return m_Input->GetY(); 
	return 0.f;
}

float InputHandle::GetDX()
{
	return m_Input->MouseDirectX();
}

float InputHandle::GetDY()
{
	return m_Input->MouseDirectY(); 
}

ControllerInput* InputHandle::GetController(u16 controller_id)
{
	return m_Controller[controller_id]; 
}

void InputHandle::CallFunction(u32 hash)
{
	if (m_Functions.find(hash) != m_Functions.end())
	{
		m_Functions[hash]();
	}
}
