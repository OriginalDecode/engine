#pragma once

namespace debug
{

	template<typename T>
	struct DebugSlider
	{
		DebugSlider() = default;
		DebugSlider(T min, T max, T* pValue, const std::string& label)
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

	template<typename T>
	struct DebugButton
	{
		DebugButton() = default;
		DebugButton(float width, float height, T* pValue, const std::string& label)
			: m_Width(width)
			, m_Height(height)
			, m_Bool(pBool)
			, m_Label(label)
		{
		}

		float m_Width;
		float m_Height;
		T* m_Value;
		std::string m_Label;
	};

	
};