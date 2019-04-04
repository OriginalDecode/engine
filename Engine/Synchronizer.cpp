#include "stdafx.h"
#include "Synchronizer.h"
#include <thread>
#include "profile_defines.h"

void Synchronizer::SwapBuffer()
{
	m_CurrentBuffer ^= 1;
}

void Synchronizer::Quit()
{
	m_LogicDone = true;
	m_RenderDone = true;
	m_QuitFlag = true;
}

void Synchronizer::WaitForRender()
{
	PROFILE_FUNCTION(profiler::colors::Amber);
	while (!m_RenderDone)
		std::this_thread::sleep_for(std::chrono::microseconds(1));

	m_RenderDone = false;
}

void Synchronizer::WaitForLogic()
{
	PROFILE_FUNCTION(profiler::colors::Amber100);
	while (!m_LogicDone)
		std::this_thread::sleep_for(std::chrono::microseconds(1));

	m_LogicDone = false;
}