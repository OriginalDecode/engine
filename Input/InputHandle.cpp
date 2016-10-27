#include "InputHandle.h"


#include <fstream>
#include <iostream>
#include <sstream>

#include "../CommonLib/DataStructures/Hashmap/Hash.h"
#include "../Engine/Engine.h"

#include "hashlist.h"

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
}

void InputHandle::HandleInput()
{
	if (m_Controller->IsConnected())
	{
		if (m_Controller->ButtonDown(eXboxButton::X)) m_XButton();
		if (m_Controller->ButtonDown(eXboxButton::Y)) m_YButton();
		if (m_Controller->ButtonDown(eXboxButton::B)) m_BButton();
		if (m_Controller->ButtonDown(eXboxButton::A)) m_AButton();
	}

	if (m_Input->Update())
	{
		if (m_Input->KeyDown(KButton::SPACE))  m_SpaceBar();
		if (m_Input->KeyDown(KButton::W)) m_WKey();
		if (m_Input->KeyDown(KButton::S)) m_SKey();
		if (m_Input->KeyDown(KButton::A)) m_AKey();
		if (m_Input->KeyDown(KButton::D)) m_DKey();
	}
}

void InputHandle::Update()
{
	if (m_Controller->LeftThumbstickY() > 0.5f) m_LThumbYP();
	if (m_Controller->LeftThumbstickY() < -0.5f) m_LThumbYN();
	if (m_Controller->LeftThumbstickX() > 0.5f) m_LThumbXP();
	if (m_Controller->LeftThumbstickX() < -0.5f) m_LThumbXN();
}

void InputHandle::Bind(u32 hash, std::function<void()> function)
{
	//Generate a new header file and add a separate file for the bind switch case?
	switch (hash)
	{
		case s_WKey_hash:
		{
			BindWKey(function);
		}break;
		case s_SKey_hash:
		{
			BindSKey(function);
		}break;
		case s_AKey_hash:
		{
			BindAKey(function);
		}break;
		case s_DKey_hash:
		{
			BindDKey(function);
		}break;
		default:
		break;
	}
}
