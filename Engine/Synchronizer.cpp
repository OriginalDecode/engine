#include "stdafx.h"
#include "Synchronizer.h"
#include <thread>

bool Synchronizer::Initiate()
{
	/*m_AllocationAmt = (0x80000 * 2);
	m_MemorySize = m_AllocationAmt * (eBufferType::BUFFER_COUNT * 2);
	*/
	//m_MainBlock = MemoryBlock(m_AllocationAmt * (eBufferType::BUFFER_COUNT * 2), m_AllocationAmt);


	const s32 allocation_amt = 0x80000 * 2;
	const s32 alloc_size = allocation_amt * ( eBufferType::BUFFER_COUNT * 2 );
	m_MainMemory = malloc(alloc_size);
	m_Allocator = CommandAllocator(alloc_size, m_MainMemory);






	/*RegisterBuffer<ModelCommand>(MODEL_BUFFER);
	RegisterBuffer<SpotlightCommand>(SPOTLIGHT_BUFFER);
	RegisterBuffer<ParticleCommand>(PARTICLE_BUFFER);
	RegisterBuffer<LineCommand>(LINE_BUFFER);
	RegisterBuffer<PointlightCommand>(POINTLIGHT_BUFFER);
	RegisterBuffer<SpriteCommand>(SPRITE_BUFFER);
	RegisterBuffer<TextCommand>(TEXT_BUFFER);*/


	m_CommandBuffers[MODEL_BUFFER][0]		=	CommandAllocator(allocation_amt, sizeof(ModelCommand)		, m_Allocator.Alloc(allocation_amt));
	m_CommandBuffers[SPOTLIGHT_BUFFER][0]	=	CommandAllocator(allocation_amt, sizeof(SpotlightCommand)	, m_Allocator.Alloc(allocation_amt));
	m_CommandBuffers[PARTICLE_BUFFER][0]	=	CommandAllocator(allocation_amt, sizeof(ParticleCommand)	, m_Allocator.Alloc(allocation_amt));
	m_CommandBuffers[LINE_BUFFER][0]		=	CommandAllocator(allocation_amt, sizeof(LineCommand)		, m_Allocator.Alloc(allocation_amt));
	m_CommandBuffers[POINTLIGHT_BUFFER][0]	=	CommandAllocator(allocation_amt, sizeof(PointlightCommand)	, m_Allocator.Alloc(allocation_amt));
	m_CommandBuffers[SPRITE_BUFFER][0]		=	CommandAllocator(allocation_amt, sizeof(SpriteCommand)		, m_Allocator.Alloc(allocation_amt));
	m_CommandBuffers[TEXT_BUFFER][0]		=	CommandAllocator(allocation_amt, sizeof(TextCommand)		, m_Allocator.Alloc(allocation_amt));
	m_CommandBuffers[MODEL_BUFFER][1]		=	CommandAllocator(allocation_amt, sizeof(ModelCommand)		, m_Allocator.Alloc(allocation_amt));
	m_CommandBuffers[SPOTLIGHT_BUFFER][1]	=	CommandAllocator(allocation_amt, sizeof(SpotlightCommand)	, m_Allocator.Alloc(allocation_amt));
	m_CommandBuffers[PARTICLE_BUFFER][1]	=	CommandAllocator(allocation_amt, sizeof(ParticleCommand)	, m_Allocator.Alloc(allocation_amt));
	m_CommandBuffers[LINE_BUFFER][1]		=	CommandAllocator(allocation_amt, sizeof(LineCommand)		, m_Allocator.Alloc(allocation_amt));
	m_CommandBuffers[POINTLIGHT_BUFFER][1]	=	CommandAllocator(allocation_amt, sizeof(PointlightCommand)	, m_Allocator.Alloc(allocation_amt));
	m_CommandBuffers[SPRITE_BUFFER][1]		=	CommandAllocator(allocation_amt, sizeof(SpriteCommand)		, m_Allocator.Alloc(allocation_amt));
	m_CommandBuffers[TEXT_BUFFER][1]		=	CommandAllocator(allocation_amt, sizeof(TextCommand)		, m_Allocator.Alloc(allocation_amt));




	return true;
}

void Synchronizer::SwapBuffer()
{
	for ( CommandBuffer& buffer : m_CommandBuffers )
	{
		buffer[m_CurrentBuffer].Clear();
	}
	m_CurrentBuffer ^= 1;
}

void Synchronizer::Quit()
{
	//m_MainBlock.CleanUp();
	m_Allocator.CleanUp();
	free(m_MainMemory);
	m_LogicDone = true;
	m_RenderDone = true;
	m_QuitFlag = true;
}

void Synchronizer::WaitForRender()
{
	while ( !m_RenderDone )
	{
		std::this_thread::yield();
	}
	m_RenderDone = false;
}

void Synchronizer::WaitForLogic()
{
	while ( !m_LogicDone )
	{
		std::this_thread::yield();
	}
	m_LogicDone = false;
}

void Synchronizer::RenderIsDone()
{
	m_RenderDone = true;
}

void Synchronizer::LogicIsDone()
{
	m_LogicDone = true;
}

const CommandAllocator& Synchronizer::GetRenderCommands(const eBufferType& buffer_type) const
{
	return m_CommandBuffers[buffer_type][m_CurrentBuffer];
}