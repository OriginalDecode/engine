#pragma once
#include <sstream>
#include <functional>
#include <vector>
#include <deque>

class InputHandle;
namespace Snowblind
{
	class CSynchronizer;
	class DebugMenu
	{
	public:
		DebugMenu() = default;
		bool Initiate(CSynchronizer* synchronizer, InputHandle* input_handle);
		void Activate() { m_IsActive = true; }
		void Deactivate() { m_IsActive = false; }
		bool GetIsActive() { return m_IsActive; }

		void AddToMenu(std::string item_name, std::function<void()> item_function);
		void Render();
		void Update();
	private:
		struct Menu_Item
		{
			Menu_Item(std::string item_name, std::function<void()> item_function)
				: m_ItemName(item_name)
				, m_Function(item_function)
			{
			}
			std::string m_ItemName;
			std::function<void()> m_Function;
		};

		bool m_IsActive = false;
		CSynchronizer* m_Synchronizer = nullptr;
		std::vector<Menu_Item> m_MenuItems;
		std::stringstream m_Stream;
		InputHandle* m_InputHandle = nullptr;
	};

	class DebugSystem
	{
	public:
		DebugSystem() = default;
		bool Initiate(CSynchronizer* synchronizer, InputHandle* input_handle);
		void Update();
		void Render();
		void ActivateDebugMenu();
		void DeactivateDebugMenu();
		bool GetDebugMenuIsActive() { return m_DebugMenu.GetIsActive(); }
		void AddDebugMenuItem(const std::string& item_name, std::function<void()> function) { m_DebugMenu.AddToMenu(item_name, function); }
		void AddToErrorList(std::string error);
		void AddToDebugText(std::string debug_text);
	private:
		DebugMenu m_DebugMenu;
		std::deque<std::string> m_ErrorMessages;
		std::vector<std::string> m_DebugStrings;
		CSynchronizer* m_Synchronizer = nullptr;
	};
};