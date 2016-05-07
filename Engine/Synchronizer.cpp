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
		my2DCommandBuffer[myCurrentBuffer].RemoveAll();
		my3DCommandBuffer[myCurrentBuffer].RemoveAll();
		myLightCommandBuffer[myCurrentBuffer].RemoveAll();
		myCurrentBuffer ^= 1;
	}

	void CSynchronizer::Clear()
	{
		my2DCommandBuffer[0].RemoveAll();
		my2DCommandBuffer[1].RemoveAll();
		my3DCommandBuffer[0].RemoveAll();
		my3DCommandBuffer[1].RemoveAll();
		myLightCommandBuffer[0].RemoveAll();
		myLightCommandBuffer[1].RemoveAll();
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
		switch (aRenderCommand.myCommandType)
		{
		case eCommandType::e2D:
			my2DCommandBuffer[myCurrentBuffer ^ 1].Add(aRenderCommand);
			break;
		case eCommandType::e3D:
			my3DCommandBuffer[myCurrentBuffer ^ 1].Add(aRenderCommand);
			break;
		case eCommandType::LIGHT:
			myLightCommandBuffer[myCurrentBuffer ^ 1].Add(aRenderCommand);
		}
	}

	const CU::GrowingArray<SRenderCommand>& CSynchronizer::GetRenderCommands(const eCommandType& commandType) const
	{
		if (commandType == eCommandType::e2D)
		{
			return my2DCommandBuffer[myCurrentBuffer];
		}

		if (commandType == eCommandType::LIGHT)
		{
			return myLightCommandBuffer[myCurrentBuffer];
		}
		return my3DCommandBuffer[myCurrentBuffer];
	}
};