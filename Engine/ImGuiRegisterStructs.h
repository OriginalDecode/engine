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
	struct DebugTextValue
	{
		DebugTextValue() = default;
		DebugTextValue(T* pValue, const std::string& label)
			: m_Value(pValue)
			, m_Label(label)
		{
		}

		T* m_Value = nullptr;
		std::string m_Label;

	};
	

	struct DebugCheckbox
	{
		DebugCheckbox() = default;
		DebugCheckbox(bool* pToggleable, const std::string& label)
			: m_Toggleable(pToggleable)
			, m_Label(label)
		{

		}
		bool* m_Toggleable;
		std::string m_Label;
	};



	
	/**
	template<typename T>
	struct DebugButton
	{
		DebugButton() = default;
		DebugButton(float width, float height, T* pValue, const std::string& label)
			: m_Width(width)
			, m_Height(height)
			, m_Value(pValue)
			, m_Label(label)
		{
		}

		float m_Width;
		float m_Height;
		T* m_Value;
		std::string m_Label;
	};

	struct DebugFunctionButton
	{
		DebugFunctionButton() = default;
		DebugFunctionButton(float width, float height, std::function<void()> pValue, const std::string& label)
			: m_Width(width)
			, m_Height(height)
			, m_Value(pValue)
			, m_Label(label)
		{
		}

		float m_Width;
		float m_Height;
		std::function<void()> m_Value;
		std::string m_Label;
	};


	struct DebugWindow
	{
		DebugWindow(float width, float height, float x_pos, float y_pos, const std::string& label)
			: m_Width(width)
			, m_Height(height)
			, m_XPos(x_pos)
			, m_YPos(y_pos)
		{
		}

		std::string m_Label;
		float m_Width = 0.f;
		float m_Height = 0.f;
		float m_XPos = 0.f;
		float m_YPos = 0.f;

		CU::GrowingArray<DebugSlider<float>> m_FloatSliders;
		CU::GrowingArray<DebugSlider<int>> m_IntSliders;
		CU::GrowingArray<DebugButton<bool>> m_BoolButtons;
		CU::GrowingArray<DebugFunctionButton> m_FunctionButtons;
	};

	
	struct DebugMainWindow
	{
	
		DebugMainWindow(float width, float height, float x_pos, float y_pos, const std::string& label)
			: m_Width(width)
			, m_Height(height)
			, m_XPos(x_pos)
			, m_YPos(y_pos)
		{
		}

		std::string m_Label;
		float m_Width = 0.f;
		float m_Height = 0.f;
		float m_XPos = 0.f;
		float m_YPos = 0.f;
		

		void RegisterChildWindow(DebugWindow window)
		{
			m_Windows.Add(window);
		};

		void RegisterChildWindow(float width, float height, float x_pos, float y_pos, const std::string& label)
		{
			RegisterChildWindow(DebugWindow(width, height, x_pos, y_pos, label));
		};


		CU::GrowingArray<DebugWindow> m_Windows;
	};*/


};