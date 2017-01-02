#include "InputHandle.h"
#include <hashlist.h>
#include "../Engine/Engine.h"

#include "ControllerInput.h"
#include "InputWrapper.h"

bool InputHandle::Initiate(HWND window_handle, HINSTANCE window_instance)
{

	Hex::Engine* engine = Hex::Engine::GetInstance();
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
	if (!Hex::Engine::GetInstance()->IsWindowActive())
		return;
	/*
	for (s32 i = 0; i < m_ControllerID; i++)
	{
		m_Controller[i]->Update();
	}
	*/
	m_Input->Update();
}

void InputHandle::HandleInput()
{
	/*if(Hex::Engine::GetInstance()->IsWindowActive())
	{
		if (m_Controller->IsConnected())
		{
			const ControllerState& state = m_Controller->GetState();
			const ControllerState& prev_state = m_Controller->GetPrevState();
			if (state.m_Buttons & x_X)
			{
				CallFunction(s_XButton_hash);
			}
			else if ((state.m_Buttons & x_Y) == 0 && (prev_state.m_Buttons & x_Y) != 0)
			{
				CallFunction(s_YButton_hash);
			}
			else if ((state.m_Buttons & x_B) && !(prev_state.m_Buttons & x_B))
			{
				CallFunction(s_BButton_hash);
			}
			else if (state.m_Buttons & x_A)
			{
				CallFunction(s_AButton_hash);
			}
			else if (state.m_Buttons & x_LB)
				CallFunction(s_LBumper_hash);
			else if (state.m_Buttons & x_RB)
				CallFunction(s_RBumper_hash);

			const float ly_value = (float)state.m_ThumbLY / SHRT_MAX;
			const float lx_value = (float)state.m_ThumbLX / SHRT_MAX;

			if (ly_value > 0.5f)
			{
				CallFunction(s_LThumbYP_hash);
			}
			else if (ly_value < -0.5f)
			{
				CallFunction(s_LThumbYN_hash);
			}

			if (lx_value > 0.5f)
			{
				CallFunction(s_LThumbXP_hash);
			}
			else if (lx_value < -0.5f)
			{
				CallFunction(s_LThumbXN_hash);
			}

			const float ry_value = (float)state.m_ThumbRY / SHRT_MAX;
			const float rx_value = (float)state.m_ThumbRX / SHRT_MAX;



			if (rx_value > 0.5f)
			{
				CallFunction(s_RThumbYP_hash);
			}
			else if (rx_value < -0.5f)
			{
				CallFunction(s_RThumbYN_hash);
			}

			if (ry_value > 0.5f)
			{
				CallFunction(s_RThumbXP_hash);
			}
			else if (ry_value < -0.5f)
			{
				CallFunction(s_RThumbXN_hash);
			}
		}

		if (m_Input->Update())
		{
			if (m_Input->KeyClick(KButton::SPACE))
			{
				CallFunction(s_SpaceBar_hash);
			}
			if (m_Input->KeyDown(KButton::W))
			{
				CallFunction(s_WKey_hash);
			}
			if (m_Input->KeyDown(KButton::S))
			{
				CallFunction(s_SKey_hash);
			}
			if (m_Input->KeyDown(KButton::A))
			{
				CallFunction(s_AKey_hash);
			}
			if (m_Input->KeyDown(KButton::D))
			{
				CallFunction(s_DKey_hash);
			}
			if (m_Input->MouseDown(0))
				CallFunction(s_LMouseButton_hash);
			if (m_Input->KeyDown(KButton::NUMADD))
				CallFunction(s_NumpadAdd_hash);
			if (m_Input->KeyDown(KButton::NUMMINUS))
				CallFunction(s_NumpadMinus_hash);
			if (m_Input->KeyClick(KButton::TAB))
				CallFunction(s_TabKey_hash);
			if (m_Input->KeyClick(KButton::F9))
				CallFunction(s_F9Key_hash);
			CallFunction(s_MoveMouse_hash);

		}
	}*/
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

void InputHandle::CallFunction(u32 hash)
{
	if (m_Functions.find(hash) != m_Functions.end())
	{
		m_Functions[hash]();
	}
}
