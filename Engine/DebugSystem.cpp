#include "stdafx.h"
#include "DebugSystem.h"
#include "Synchronizer.h"
#include "RenderCommand.h"

bool Snowblind::DebugMenu::Initiate(CSynchronizer* synchronizer)
{
	m_Synchronizer = synchronizer;
	return true;
}

void Snowblind::DebugMenu::AddToMenu(std::string item_name, std::function<void()> item_function)
{
	m_MenuItems.push_back(Menu_Item(item_name, item_function));
}

void Snowblind::DebugMenu::Render()
{
	for (const Menu_Item& item_name : m_MenuItems)
	{
		std::string toPrint = item_name.m_ItemName;
		m_Synchronizer->AddRenderCommand(RenderCommand(toPrint, { 0, 0 }, eType::TEXT));
	}
}

bool Snowblind::DebugSystem::Initiate(CSynchronizer* synchronizer)
{
	m_Synchronizer = synchronizer;
	m_DebugMenu.Initiate(m_Synchronizer);

	return true;
}

void Snowblind::DebugSystem::Render()
{
	if (m_DebugMenu.GetIsActive())
	{
		m_DebugMenu.Render();
	}
}

void Snowblind::DebugSystem::ActivateDebugMenu()
{
	m_DebugMenu.Activate();
}

void Snowblind::DebugSystem::DeactivateDebugMenu()
{
	m_DebugMenu.Deactivate();
}



