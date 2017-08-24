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

		void RegisterBoolButton(float width, float height, bool* pBool, const std::string& label);
		void RegisterFunctionButton(float width, float height, std::function<void()> pFunc, const std::string& label);


	private:


		void HandleIntSliders();
		void HandleFloatSliders();

		void HandleFunctionButtons();
		void HandleBoolButtons();


		CU::GrowingArray<DebugSlider<float>> m_FloatSliders;
		CU::GrowingArray<DebugSlider<int>> m_IntSliders;
		CU::GrowingArray<DebugButton<bool>> m_BoolButtons;
		CU::GrowingArray<DebugFunctionButton> m_FunctionButtons;
	};
};