#pragma once
#include <sstream>
#include <functional>
#include <vector>
namespace Snowblind
{
	class CSynchronizer;


	class DebugMenu
	{
	public:
		DebugMenu() = default;
		bool Initiate(CSynchronizer* synchronizer);
		void Activate() { m_IsActive = true; }
		void Deactivate() { m_IsActive = false; }
		bool GetIsActive() { return m_IsActive; }

		void AddToMenu(std::string item_name, std::function<void()> item_function);
		void Render();
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
	};

	class DebugSystem
	{
	public:
		DebugSystem() = default;
		bool Initiate(CSynchronizer* synchronizer);
		void Render();
		void ActivateDebugMenu();
		void DeactivateDebugMenu();
		bool GetDebugMenuIsActive() { return m_DebugMenu.GetIsActive(); }
		void AddDebugMenuItem(const std::string& item_name, std::function<void()> function) { m_DebugMenu.AddToMenu(item_name, function); }

	private:
		DebugMenu m_DebugMenu;
		CSynchronizer* m_Synchronizer = nullptr;
	};
};