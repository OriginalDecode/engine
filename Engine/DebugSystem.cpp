#include "stdafx.h"
#include "DebugSystem.h"
#include "Synchronizer.h"
#include "RenderCommand.h"
#include "../Input/InputHandle.h"
constexpr s16 max_error_count = 32;

namespace Snowblind
{
	bool DebugMenu::Initiate(InputHandle* input_handle)
	{
		m_Synchronizer = Engine::GetInstance()->GetSynchronizer();
		m_InputHandle = input_handle;
		return true;
	}

	void DebugMenu::AddToMenu(std::string item_name, std::function<void()> item_function)
	{
		m_MenuItems.push_back(Menu_Item(item_name, item_function));
	}

	void DebugMenu::Render()
	{
		for (const Menu_Item& item_name : m_MenuItems)
		{
			std::string toPrint = item_name.m_ItemName;
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::TEXT, toPrint, CU::Vector2f(0, 0)));
		}
	}

	void DebugMenu::Update()
	{
		m_InputHandle->HandleInput();
	}

	bool DebugSystem::Initiate(InputHandle* input_handle)
	{
		m_Synchronizer = Engine::GetInstance()->GetSynchronizer();
		m_DebugMenu.Initiate(input_handle);

		return true;
	}

	void DebugSystem::Update()
	{
		m_DebugMenu.Update();
	}

	void DebugSystem::Render()
	{
		if (m_DebugMenu.GetIsActive())
		{
			m_DebugMenu.Render();
		}

		for (s32 i = 0; i < m_ErrorMessages.size(); i++)
		{
			m_Synchronizer->AddRenderCommand(RenderCommand(eType::TEXT, m_ErrorMessages[i], CU::Vector2f(0.5f, (1.f - (i != 0) ? 1.f / i : 0.f))));
		}

		std::stringstream debug_text;
		for (s32 i = 0; i < m_DebugStrings.size(); i++)
		{
			debug_text << m_DebugStrings[i] << "\n";
		}

		m_Synchronizer->AddRenderCommand(RenderCommand(eType::TEXT, debug_text.str(), CU::Vector2f(0.f, 0.f)));
		m_DebugStrings.clear();
	}

	void DebugSystem::ActivateDebugMenu()
	{
		m_DebugMenu.Activate();
	}

	void DebugSystem::DeactivateDebugMenu()
	{
		m_DebugMenu.Deactivate();
	}

	void DebugSystem::AddToErrorList(std::string error)
	{
		if (m_ErrorMessages.size() > max_error_count)
		{
			m_ErrorMessages.pop_front();
		}
		m_ErrorMessages.push_back(error);
	}

	void DebugSystem::AddToDebugText(std::string debug_text)
	{
		m_DebugStrings.push_back(debug_text);
	}
};