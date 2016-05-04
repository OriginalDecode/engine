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
		myCommandBuffer[myCurrentBuffer].RemoveAll();
		myCurrentBuffer ^= 1;
	}

	void CSynchronizer::Clear()
	{
		myCommandBuffer[0].RemoveAll();
		myCommandBuffer[1].RemoveAll();
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
		myCommandBuffer[myCurrentBuffer ^ 1].Add(aRenderCommand);
	}

	const CU::GrowingArray<SRenderCommand>& CSynchronizer::GetRenderCommands() const
	{
		return myCommandBuffer[myCurrentBuffer];
	}

};