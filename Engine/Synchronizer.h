#pragma once

#include <bitset>

#include <DataStructures/StaticArray.h>
#include <DataStructures/GrowingArray.h>

#include "RenderCommand.h"

namespace Snowblind
{


	class CSynchronizer
	{
	public:
		CSynchronizer();
		~CSynchronizer();
		void SwapBuffer();
		void Clear();
		void Quit();
		bool HasQuit();
		void WaitForRender();
		void WaitForLogic();
		void RenderIsDone();
		void LogicIsDone();
		void AddRenderCommand(const SRenderCommand& aRenderCommand);
		const CU::GrowingArray<SRenderCommand>& GetRenderCommands() const;
	private:
		CU::StaticArray<CU::GrowingArray<SRenderCommand>, 2> myCommandBuffer;

		volatile bool myLogicIsDone;
		volatile bool myRenderIsDone;
		volatile bool myQuitFlag;
		bool myCurrentBuffer; //this works. And  it is only 1 byte.

	};

	__forceinline bool CSynchronizer::HasQuit()
	{
		return myQuitFlag;
	}

};