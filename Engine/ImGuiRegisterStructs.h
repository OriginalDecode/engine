#pragma once

namespace debug
{

	template<typename T>
	struct ImGuiSlider
	{
		ImGuiSlider(T min, T max, T* pValue, const std::string& label)
			: m_Min(min)
			, m_Max(max)
			, m_Value(pValue)
			, m_Label(label)
		{
		}

		T m_Min = 0;
		T m_Max = 0;
		T* m_Value = nullptr;
		std::string m_Label; 
	};


// 	struct ImGuiFloatSlider
// 	{
// 		ImGuiFloatSlider(float min, float max, float* pValue)
// 			: m_Min(min)
// 			, m_Max(max)
// 			, m_Value(pValue)
// 		{
// 		}
// 		float m_Min = 0.f;
// 		float m_Max = 1.f;
// 		float* m_Value = nullptr;
// 	};


};