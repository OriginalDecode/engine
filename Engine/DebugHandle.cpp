#include "stdafx.h"
#include "DebugHandle.h"

#if !defined(_PROFILE) && !defined(_FINAL)
#include "imgui.h"
#endif
#ifdef _PROFILE
#include <easy/profiler.h>
#include <easy/reader.h>
#endif

namespace debug
{
	static bool s_Open = true;
	void DebugHandle::Update()
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(300, Engine::GetInstance()->GetInnerSize().m_Height));
		ImGuiWindowFlags flags = 0;
		flags |= ImGuiWindowFlags_NoTitleBar;
		flags |= ImGuiWindowFlags_NoResize;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		if (ImGui::Begin("Information", &s_Open, flags))
		{
			HandleIntSliders();
			HandleFloatSliders();
			HandleFunctionButtons();

			ImGui::End();
		}
		ImGui::PopStyleVar();
	}

	//_______________________________________________________________
	void DebugHandle::HandleFloatSliders()
	{
		for (const auto& s : m_FloatSliders)
		{
			ImGui::SliderFloat(s.m_Label.c_str(), s.m_Value, s.m_Min, s.m_Max);
		}
	}

	//_______________________________________________________________
	void DebugHandle::HandleIntSliders()
	{
		for (const auto& s : m_IntSliders)
		{
			ImGui::SliderInt(s.m_Label.c_str(), s.m_Value, s.m_Min, s.m_Max);
		}
	}

	//_______________________________________________________________
	void DebugHandle::HandleFunctionButtons()
	{
		for (const auto& s : m_FunctionButtons)
		{
			if (ImGui::Button(s.m_Label.c_str(), ImVec2(s.m_Width, s.m_Height)))
			{
				s.m_Value();
			}
		}
	}

	//_______________________________________________________________
	void DebugHandle::HandleBoolButtons()
	{
		for (const auto& s : m_BoolButtons)
		{
		}
	}

	//_______________________________________________________________
	void DebugHandle::RegisterIntSlider(int min, int max, int* pValue, const std::string& label)
	{
		m_IntSliders.Add(DebugSlider<int>(min, max, pValue, label));
	}

	//_______________________________________________________________
	void DebugHandle::RegisterBoolButton(float width, float height, bool* pBool, const std::string& label)
	{

	}

	//_______________________________________________________________
	void DebugHandle::RegisterFunctionButton(float width, float height, std::function<void()> pFunc, const std::string& label)
	{
		m_FunctionButtons.Add(DebugFunctionButton(width, height, pFunc, label));
	}

	//_______________________________________________________________
	void DebugHandle::RegisterFloatSlider(float min, float max, float* pValue, const std::string& label)
	{
		m_FloatSliders.Add(DebugSlider<float>(min, max, pValue, label));
	}


	//_______________________________________________________________
};