#pragma once
#include <DataStructures/StaticArray.h>
#include <DataStructures/GrowingArray.h>
#include "RenderCommand.h"
#include <standard_datatype.hpp>
#include "snowblind_shared.h"
typedef CU::StaticArray<CU::GrowingArray<RenderCommand>, 2> CommandBuffer;
typedef CU::StaticArray<CommandBuffer, static_cast<u32>(eCommandBuffer::_COUNT)> CommandBuffers;



class Synchronizer
{
public:
	Synchronizer() = default;
	bool Initiate();
	void SwapBuffer();
	void Clear();
	void Quit();
	bool HasQuit();
	void WaitForRender();
	void WaitForLogic();
	void RenderIsDone();
	void LogicIsDone();

	bool LogicHasFinished() { return myLogicIsDone; }

	void AddRenderCommand(const RenderCommand& aRenderCommand);




	const CU::GrowingArray<RenderCommand>& GetRenderCommands(const eCommandBuffer& commandType) const;
private:
	CommandBuffers myCommandBuffers;
	volatile bool myLogicIsDone;
	volatile bool myRenderIsDone;
	volatile bool myQuitFlag;
	u16 m_CurrentBuffer;

};

__forceinline bool Synchronizer::HasQuit()
{
	return myQuitFlag;
}

