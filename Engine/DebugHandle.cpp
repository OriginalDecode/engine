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

	void DebugHandle::Update()
	{

		HandleIntSliders();
		HandleFloatSliders();
	}

	void DebugHandle::RegisterFloatSlider(float min, float max, float* pValue, const std::string& label)
	{
		m_FloatSliders.Add(ImGuiSlider<float>(min, max, pValue, label));
	}

	void DebugHandle::HandleFloatSliders()
	{
		for (const auto& s : m_FloatSliders)
		{
			ImGui::SliderFloat(s.m_Label.c_str(), s.m_Value, s.m_Min, s.m_Max);
		}
	}


	void DebugHandle::RegisterIntSlider(int min, int max, int* pValue, const std::string& label)
	{
		m_IntSliders.Add(ImGuiSlider<int>(min, max, pValue, label));
	}

	void DebugHandle::HandleIntSliders()
	{
		for (const auto& s : m_IntSliders)
		{
			ImGui::SliderInt(s.m_Label.c_str(), s.m_Value, s.m_Min, s.m_Max);
		}
	}

};