#pragma once

#include "engine_shared.h"
class Synchronizer
{
public:
	Synchronizer() = default;
	~Synchronizer() = default;
	void SwapBuffer();
	void Quit();
	bool HasQuit() const { return m_QuitFlag; }

	void WaitForRender();
	void WaitForLogic();

	void RenderIsDone() { m_RenderDone = true; }
	void LogicIsDone()	{ m_LogicDone = true; }

	uint16 GetCurrentBufferIndex() const { return m_CurrentBuffer; }

	bool LogicHasFinished() { return m_LogicDone; }

private:
	volatile bool m_LogicDone = false;
	volatile bool m_RenderDone = false;
	volatile bool m_QuitFlag = false;
	uint16 m_CurrentBuffer = 0;
};
