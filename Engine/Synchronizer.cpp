#include "stdafx.h"
#include "Synchronizer.h"
#include <thread>

#define REGISTER_COMMAND_BUFFER(objBuffer, objType, objCount)\
myCommandBuffers[(u32)objBuffer][0] = MemoryBlock(objCount, sizeof(objType)); \
myCommandBuffers[(u32)objBuffer][1] = MemoryBlock(objCount, sizeof(objType));


bool Synchronizer::Initiate()
{
	REGISTER_COMMAND_BUFFER(eCommandBuffer::e3D, ModelCommand, 0x80000);
	REGISTER_COMMAND_BUFFER(eCommandBuffer::eSpotlight, SpotlightCommand, 0x80000);

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
		buffer[m_CurrentBuffer].Clear();
	}
	m_CurrentBuffer ^= 1;
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
	return;
	CommandBuffer& buffer = myCommandBuffers[u32(aRenderCommand.myCommandType)];
}

const MemoryBlock& Synchronizer::GetRenderCommands(const eCommandBuffer& buffer_type) const
{
	return myCommandBuffers[( u32 ) buffer_type][m_CurrentBuffer];
}