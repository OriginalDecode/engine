#include "stdafx.h"
#include "Synchronizer.h"
#include <thread>


namespace Snowblind
{
	CSynchronizer::CSynchronizer()
		: myCurrentBuffer(0)
		, myLogicIsDone(false)
		, myRenderIsDone(false)
		, myQuitFlag(false)
	{
	}

	CSynchronizer::~CSynchronizer()
	{
	}

	void CSynchronizer::SwapBuffer()
	{
		for (CommandBuffer& buffer : myCommandBuffers)
		{
			buffer[myCurrentBuffer].RemoveAll();
		}
		myCurrentBuffer ^= 1;
	}

	void CSynchronizer::Clear()
	{
		for (CommandBuffer& buffer : myCommandBuffers)
		{
			buffer[myCurrentBuffer].RemoveAll();
			buffer[myCurrentBuffer ^ 1].RemoveAll();
		}
	}

	void CSynchronizer::Quit()
	{
		myLogicIsDone = true;
		myRenderIsDone = true;
		myQuitFlag = true;
	}

	void CSynchronizer::WaitForRender()
	{
		while (!myRenderIsDone)
		{
			std::this_thread::yield();
		}
		myRenderIsDone = false;
	}

	void CSynchronizer::WaitForLogic()
	{
		while (!myLogicIsDone)
		{
			std::this_thread::yield();
		}
		myLogicIsDone = false;
	}

	void CSynchronizer::RenderIsDone()
	{
		myRenderIsDone = true;
	}

	void CSynchronizer::LogicIsDone()
	{
		myLogicIsDone = true;
	}

	void CSynchronizer::AddRenderCommand(const SRenderCommand& aRenderCommand)
	{
		myCommandBuffers[u32(aRenderCommand.myCommandType)][myCurrentBuffer ^ 1].Add(aRenderCommand);
	}

	const CU::GrowingArray<SRenderCommand>& CSynchronizer::GetRenderCommands(const eCommandBuffer& commandType) const
	{
		return myCommandBuffers[u32(commandType)][myCurrentBuffer];
	}
};