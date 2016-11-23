#include "InputHandle.h"
#include <hashlist.h>
#include "../Engine/Engine.h"

#include "ControllerInput.h"
#include "InputWrapper.h"

bool InputHandle::Initiate(u16 controller_ID)
{
	m_Controller = new ControllerInput(controller_ID);
	if (!m_Controller)
		return false;

	m_Input = new InputWrapper;
	if (!m_Input)
		return false;

	Snowblind::CEngine* engine = Snowblind::CEngine::GetInstance();
	if (!engine)
		return false;

	if (!m_Input->Initiate(engine->GetWindow().GetHWND(), engine->GetWindow().GetWindowInstance()))
		return false;
	
	return true;
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
	if(Snowblind::CEngine::GetInstance()->IsWindowActive())
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


		}
	}
}

void InputHandle::Bind(u32 hash, std::function<void()> function)
{
	m_Functions.emplace(hash, function);
}

float InputHandle::GetX()
{
	return m_Input->GetX();
}

float InputHandle::GetY()
{
	return m_Input->GetY(); 
}

void InputHandle::CallFunction(u32 hash)
{
	if (m_Functions.find(hash) != m_Functions.end())
	{
		m_Functions[hash]();
	}
}
