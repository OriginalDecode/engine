#include "../Engine/Engine.h"
#include "InputHandle.h"
#include "ControllerInput.h"
#include "InputWrapper.h"
#include "InputCommand.h"
void InputHandle::Initiate(u16 controller_ID)
{
	m_Controller = new ControllerInput(controller_ID);
	m_Input = new InputWrapper;
	Snowblind::CEngine* engine = Snowblind::CEngine::GetInstance();
	m_Input->Initiate(engine->GetWindow().GetHWND()
		, engine->GetWindow().GetWindowInstance());
}

void InputHandle::CleanUp()
{
	delete m_Controller;
	m_Controller = nullptr;

	delete m_Input;
	m_Input = nullptr;

	delete  m_XButton;
	delete  m_YButton;
	delete  m_BButton;
	delete  m_AButton;

	delete  m_LThumbYP;
	delete  m_LThumbYN;
	delete  m_LThumbXP;
	delete  m_LThumbXN;

}

InputCommand* InputHandle::HandleInput()
{
	if (m_Controller->IsConnected())
	{
		if (m_Controller->ButtonDown(eXboxButton::X)) return m_XButton;
		if (m_Controller->ButtonDown(eXboxButton::Y)) return m_YButton;
		if (m_Controller->ButtonDown(eXboxButton::B)) return m_BButton;
		if (m_Controller->ButtonDown(eXboxButton::A)) return m_AButton;
	}
	if (m_Input->Update())
	{
		if (m_Input->KeyDown(KButton::SPACE)) return m_SpaceBar;
		if (m_Input->KeyDown(KButton::W)) return m_WKey;
		if (m_Input->KeyDown(KButton::S)) return m_SKey;
		if (m_Input->KeyDown(KButton::A)) return m_AKey;
		if (m_Input->KeyDown(KButton::D)) return m_DKey;
	}

	return nullptr;
}

void InputHandle::Update()
{
	//Handle value operations
	if (m_Controller->LeftThumbstickY() > 0.5f)
		m_LThumbYP->m_Function();

	if (m_Controller->LeftThumbstickY() < -0.5f)
		m_LThumbYN->m_Function();

	if (m_Controller->LeftThumbstickX() > 0.5f)
		m_LThumbXP->m_Function();

	if (m_Controller->LeftThumbstickX() < -0.5f)
		m_LThumbXN->m_Function();
}
