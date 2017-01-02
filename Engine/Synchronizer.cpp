#include "stdafx.h"
#include "Synchronizer.h"
#include <thread>

namespace Hex
{
	bool Synchronizer::Initiate()
	{
		m_CurrentBuffer = 0;
		myLogicIsDone = false;
		myRenderIsDone = false;
		myQuitFlag = false;

		return true;
	}

	void Synchronizer::SwapBuffer()
	{
		for (CommandBuffer& buffer : myCommandBuffers)
		{
			buffer[m_CurrentBuffer].RemoveAll();
		}
		m_CurrentBuffer ^= 1;
	}

	void Synchronizer::Clear()
	{
		for (CommandBuffer& buffer : myCommandBuffers)
		{
			buffer[m_CurrentBuffer].RemoveAll();
			buffer[m_CurrentBuffer ^ 1].RemoveAll();
		}
	}

	void Synchronizer::Quit()
	{
		myLogicIsDone = true;
		myRenderIsDone = true;
		myQuitFlag = true;
	}

	void Synchronizer::WaitForRender()
	{
		while (!myRenderIsDone)
		{
			std::this_thread::yield();
		}
		myRenderIsDone = false;
	}

	void Synchronizer::WaitForLogic()
	{
		while (!myLogicIsDone)
		{
			std::this_thread::yield();
		}
		myLogicIsDone = false;
	}

	void Synchronizer::RenderIsDone()
	{
		myRenderIsDone = true;
	}

	void Synchronizer::LogicIsDone()
	{
		myLogicIsDone = true;
	}

	void Synchronizer::AddRenderCommand(const RenderCommand& aRenderCommand)
	{
		CommandBuffer& buffer = myCommandBuffers[u32(aRenderCommand.myCommandType)]; 
		buffer[m_CurrentBuffer ^ 1].Add(aRenderCommand);

	}

	const CU::GrowingArray<RenderCommand>& Synchronizer::GetRenderCommands(const eCommandBuffer& commandType) const
	{
		return myCommandBuffers[u32(commandType)][m_CurrentBuffer];
	}
};