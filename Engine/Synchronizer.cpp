#include "stdafx.h"
#include "Synchronizer.h"
#include <thread>

bool Synchronizer::Initiate()
{
	/*m_AllocationAmt = (0x80000 * 2);
	m_MemorySize = m_AllocationAmt * (eBufferType::BUFFER_COUNT * 2);
	*/
	//m_MainBlock = MemoryBlock(m_AllocationAmt * (eBufferType::BUFFER_COUNT * 2), m_AllocationAmt);


	const s32 model_budget = 30000;
	const s32 spotlight_budget = 30000;
	const s32 particle_budget = 30000;
	const s32 line_budget = 30000;
	const s32 pointlight_budget = 30000;
	const s32 sprite_budget = 30000;
	const s32 text_budget = 30000;


	const s32 model_buffer_size = model_budget * sizeof(ModelCommand);
	const s32 spotlight_buffer_size = spotlight_budget * sizeof(SpotlightCommand);
	const s32 particle_buffer_size = particle_budget * sizeof(ParticleCommand);
	const s32 line_buffer_size = line_budget * sizeof(LineCommand);
	const s32 pointlight_buffer_size = pointlight_budget * sizeof(PointlightCommand);
	const s32 sprite_buffer_size = sprite_budget * sizeof(SpriteCommand);
	const s32 text_buffer_size = text_budget * sizeof(TextCommand);

	const s32 total_size =
		model_buffer_size +
		spotlight_buffer_size +
		particle_buffer_size +
		line_buffer_size +
		pointlight_buffer_size +
		sprite_buffer_size +
		text_buffer_size;

	const s32 allocation_size = total_size * 2;

	const s32 allocation_amt = 0x80000 * 2;
	const s32 alloc_size = allocation_amt * (eBufferType::BUFFER_COUNT * 2);

	m_MainMemory = malloc(allocation_size);
	m_Allocator = CommandAllocator(allocation_size, m_MainMemory);

	m_CommandBuffers[MODEL_BUFFER][0] = CommandAllocator(model_buffer_size, sizeof(ModelCommand), m_Allocator.Alloc(model_buffer_size));
	m_CommandBuffers[SPOTLIGHT_BUFFER][0] = CommandAllocator(spotlight_buffer_size, sizeof(SpotlightCommand), m_Allocator.Alloc(spotlight_buffer_size));
	m_CommandBuffers[PARTICLE_BUFFER][0] = CommandAllocator(particle_buffer_size, sizeof(ParticleCommand), m_Allocator.Alloc(particle_buffer_size));
	m_CommandBuffers[LINE_BUFFER][0] = CommandAllocator(line_buffer_size, sizeof(LineCommand), m_Allocator.Alloc(line_buffer_size));
	m_CommandBuffers[POINTLIGHT_BUFFER][0] = CommandAllocator(pointlight_buffer_size, sizeof(PointlightCommand), m_Allocator.Alloc(pointlight_buffer_size));
	m_CommandBuffers[SPRITE_BUFFER][0] = CommandAllocator(sprite_buffer_size, sizeof(SpriteCommand), m_Allocator.Alloc(sprite_buffer_size));
	m_CommandBuffers[TEXT_BUFFER][0] = CommandAllocator(text_buffer_size, sizeof(TextCommand), m_Allocator.Alloc(text_buffer_size));

	m_CommandBuffers[MODEL_BUFFER][1] = CommandAllocator(model_buffer_size, sizeof(ModelCommand), m_Allocator.Alloc(model_buffer_size));
	m_CommandBuffers[SPOTLIGHT_BUFFER][1] = CommandAllocator(spotlight_buffer_size, sizeof(SpotlightCommand), m_Allocator.Alloc(spotlight_buffer_size));
	m_CommandBuffers[PARTICLE_BUFFER][1] = CommandAllocator(particle_buffer_size, sizeof(ParticleCommand), m_Allocator.Alloc(particle_buffer_size));
	m_CommandBuffers[LINE_BUFFER][1] = CommandAllocator(line_buffer_size, sizeof(LineCommand), m_Allocator.Alloc(line_buffer_size));
	m_CommandBuffers[POINTLIGHT_BUFFER][1] = CommandAllocator(pointlight_buffer_size, sizeof(PointlightCommand), m_Allocator.Alloc(pointlight_buffer_size));
	m_CommandBuffers[SPRITE_BUFFER][1] = CommandAllocator(sprite_buffer_size, sizeof(SpriteCommand), m_Allocator.Alloc(sprite_buffer_size));
	m_CommandBuffers[TEXT_BUFFER][1] = CommandAllocator(text_buffer_size, sizeof(TextCommand), m_Allocator.Alloc(text_buffer_size));




	return true;
}

void Synchronizer::SwapBuffer()
{
	for (CommandBuffer& buffer : m_CommandBuffers)
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
	while (!m_RenderDone)
	{
		std::this_thread::yield();
	}
	m_RenderDone = false;
}

void Synchronizer::WaitForLogic()
{
	while (!m_LogicDone)
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