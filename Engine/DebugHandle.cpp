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

	s32 DebugHandle::RegisterMainWindow(DebugMainWindow window)
	{
		m_MainWindows.Add(window);
	}

	s32 DebugHandle::RegisterWindow(float width, float height, float x_pos, float y_pos, const std::string& label)
	{

	}




	//_______________________________________________________________
};