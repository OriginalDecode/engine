#include "stdafx.h"
#include "Synchronizer.h"
#include <thread>

bool Synchronizer::Initiate()
{
	m_AllocationAmt = 0x80000;
	m_MemorySize = m_AllocationAmt * (eBufferType::BUFFER_COUNT * 2);
	
	m_MainBlock = MemoryBlock(m_AllocationAmt * (eBufferType::BUFFER_COUNT * 2), m_AllocationAmt);

	RegisterBuffer<ModelCommand>(MODEL_BUFFER);
	RegisterBuffer<SpotlightCommand>(SPOTLIGHT_BUFFER);
	RegisterBuffer<ParticleCommand>(PARTICLE_BUFFER);
	RegisterBuffer<LineCommand>(LINE_BUFFER);
	RegisterBuffer<PointlightCommand>(POINTLIGHT_BUFFER);
	RegisterBuffer<SpriteCommand>(SPRITE_BUFFER);
	RegisterBuffer<TextCommand>(TEXT_BUFFER);

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
	m_MainBlock.CleanUp();
	//free(m_MainMemory);
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

//void Synchronizer::AddRenderCommand(const RenderCommand& aRenderCommand)
//{
//	return;
//	CommandBuffer& buffer = myCommandBuffers[u32(aRenderCommand.myCommandType)];
//}

const MemoryBlock& Synchronizer::GetRenderCommands(const eBufferType& buffer_type) const
{
	return myCommandBuffers[buffer_type][m_CurrentBuffer];
}