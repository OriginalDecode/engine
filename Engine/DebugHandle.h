#pragma once
#include "ImGuiRegisterStructs.h"

namespace debug
{
	class DebugHandle
	{
	public:
		DebugHandle() = default;
		~DebugHandle() { }


		void Update();


		//ImGui registers
		void RegisterFloatSlider(float min, float max, float* pValue, const std::string& label);
		void RegisterIntSlider(int min, int max, int* pValue, const std::string& label);


	private:


		void HandleIntSliders();
		void HandleFloatSliders();


		CU::GrowingArray<ImGuiSlider<float>> m_FloatSliders;
		CU::GrowingArray<ImGuiSlider<int>> m_IntSliders;

	};
};