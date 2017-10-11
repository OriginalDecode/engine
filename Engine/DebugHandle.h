#pragma once
#include "ImGuiRegisterStructs.h"
#include <CommonLib/DataStructures/GrowingArray.h>
class Texture;
namespace debug
{
	class DebugHandle
	{
	public:
		static void Create() { m_Instance = new DebugHandle; }
		static void Destroy() { delete m_Instance; m_Instance = nullptr; }
		static DebugHandle* GetInstance() { return m_Instance; }
		void Update();

		void DebugTextures();
		void AddTexture(void* srv, const std::string& debug_name);
		void AddTexture(Texture* texture, const std::string& debug_name);
		void RegisterFloatSlider(DebugSlider<float> slider);
	private:

		static DebugHandle* m_Instance;

		DebugHandle() = default;
		~DebugHandle() { }

		CU::GrowingArray<DebugSlider<float>> m_Sliders;


		/**
#if !defined(_PROFILE) && !defined(_FINAL)
		bool SaveLevel();
		bool GetLineRendering();
		void EditEntity();
		void DebugTextures();
		void AddTexture(Texture* texture, const std::string& debug_name);
		void AddTexture(void* srv, const std::string& debug_name);

		void RegisterCheckBox(bool* pBool, const std::string& box_name)
		{
			CheckBox box;
			box.m_Name = box_name;
			box.m_Toggle = pBool;
			m_Checkboxes.Add(box);
		}

		void AddFunction(const std::string& label, std::function<void()> function);
		void AddCheckBox(bool* toggle, std::string label);
		void RegisterFloatSider(float* v, const char* label, float min, float max);
		CU::GrowingArray<void*>& GetDebugTextures() { return m_DebugTextures; }
	private:
		struct CheckBox
		{
			std::string m_Name;
			bool* m_Toggle = false;
		};
		CU::GrowingArray<CheckBox> m_Checkboxes;
		struct slider
		{
			float* current_value;
			const char* label;
			float min = 0.f;
			float max = 1.f;
		};
		CU::GrowingArray<slider> m_Sliders;
		void UpdateDebugUI();
		std::vector<std::string> m_Levels;
		typedef std::function<void()> callback;
		std::vector<std::pair<std::string, callback>> m_Functions;

#endif
		*/
		CU::GrowingArray<void*> m_DebugTextures;
		std::vector<std::string> m_Labels;

	};
};