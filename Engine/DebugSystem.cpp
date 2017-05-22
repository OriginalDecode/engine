#include "stdafx.h"
#include "DebugSystem.h"
#include "Synchronizer.h"
#include "RenderCommand.h"
#include "../Input/InputHandle.h"

constexpr s16 max_error_count = 32;

bool DebugPrinter::Initiate(InputHandle* input_handle)
{
	m_Engine = Engine::GetInstance();
	m_Synchronizer = m_Engine->GetSynchronizer();

	return true;
}

void DebugPrinter::Update()
{
}

void DebugPrinter::Render()
{
	for (s32 i = 0; i < m_ErrorMessages.size(); i++)
	{
		//m_Synchronizer->AddRenderCommand(RenderCommand(eType::TEXT, m_ErrorMessages[i], CU::Vector2f(0.5f, (1.f - (i != 0) ? 1.f / i : 0.f))));
	}

	std::stringstream debug_text;
	for (s32 i = 0; i < m_DebugStrings.size(); i++)
	{
		debug_text << m_DebugStrings[i] << "\n";
	}

	//m_Synchronizer->AddRenderCommand(RenderCommand(eType::TEXT, debug_text.str(), CU::Vector2f(0.f, 0.f)));
	m_DebugStrings.clear();
}


void DebugPrinter::AddToErrorList(std::string error)
{
	if (m_ErrorMessages.size() > max_error_count)
	{
		m_ErrorMessages.pop_front();
	}
	m_ErrorMessages.push_back(error);
}

void DebugPrinter::AddToDebugText(std::string debug_text)
{
	m_DebugStrings.push_back(debug_text);
}
