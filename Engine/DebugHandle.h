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
		void RegisterFloatSlider(s32 window_id, float min, float max, float* pValue, const std::string& label);
		void RegisterIntSlider(s32 window_id, int min, int max, int* pValue, const std::string& label);

		void RegisterBoolButton(s32 window_id, float width, float height, bool* pBool, const std::string& label);
		void RegisterFunctionButton(s32 window_id, float width, float height, std::function<void()> pFunc, const std::string& label);

		s32 RegisterMainWindow(DebugMainWindow window);
		s32 RegisterMainWindow(float width, float height, float x_pos, float y_pos, const std::string& label);

		s32 RegisterMainWindow(DebugMainWindow window);
		s32 RegisterMainWindow(float width, float height, float x_pos, float y_pos, const std::string& label);


	private:


		void HandleIntSliders();
		void HandleFloatSliders();

		void HandleFunctionButtons();
		void HandleBoolButtons();

		CU::GrowingArray<DebugMainWindow> m_MainWindows;


		CU::GrowingArray<DebugSlider<float>> m_FloatSliders;
		CU::GrowingArray<DebugSlider<int>> m_IntSliders;
		CU::GrowingArray<DebugButton<bool>> m_BoolButtons;
		CU::GrowingArray<DebugFunctionButton> m_FunctionButtons;
	};
};