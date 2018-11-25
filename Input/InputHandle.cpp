#include "InputHandle.h"
#include "../Engine/Engine.h"

#include "ControllerInput.h"
#include "InputWrapper.h"

InputHandle::~InputHandle()
{
	for (ControllerInput* c : m_Controller)
	{
		SAFE_DELETE(c);
	}
	SAFE_DELETE(m_Input);
}

bool InputHandle::Initiate(HWND window_handle, HINSTANCE window_instance)
{

	Engine* engine = Engine::GetInstance();
	if (!engine)
		return false;

	m_Input = new InputWrapper;
	if (!m_Input)
		return false;

	if (!m_Input->Initiate(window_handle, window_instance))
	{
		SAFE_DELETE(m_Input);
		return false;
	}
	return true;
}

void InputHandle::AddController(u16 controller_id)
{
#ifdef _DEBUG
	char buf[128];
	memset(&buf, 0, 128);
	sprintf(buf, "Can't add more than %d controllers", m_ControllerMaxAmount);
	ASSERT(m_ControllerID < m_ControllerMaxAmount, buf);
#endif
	m_Controller[m_ControllerID++] = new ControllerInput(controller_id);
	ASSERT(m_Controller[controller_id], "Failed to create a controller!");
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
