#include "stdafx.h"
#include "Synchronizer.h"
#include <thread>

bool Synchronizer::Initiate()
{
	const s32 model_buffer_size = 30000 * sizeof(ModelCommand);
	const s32 spotlight_buffer_size = 30000 * sizeof(SpotlightCommand);
	const s32 particle_buffer_size = 30000 * sizeof(ParticleCommand);
	const s32 line_buffer_size = 30000 * sizeof(LineCommand);
	const s32 pointlight_buffer_size = 30000 * sizeof(PointlightCommand);
	const s32 sprite_buffer_size = 30000 * sizeof(SpriteCommand);
	const s32 text_buffer_size = 30000 * sizeof(TextCommand);

	const s32 half_chunk_size =
		model_buffer_size +
		model_buffer_size +
		spotlight_buffer_size +
		particle_buffer_size +
		line_buffer_size +
		pointlight_buffer_size +
		sprite_buffer_size +
		text_buffer_size;

	const s32 chunk_size = half_chunk_size * 2;
	m_MainMemory = malloc(chunk_size);
	memory::LinearAllocator allocator = memory::LinearAllocator(chunk_size, m_MainMemory);

	m_CommandBuffers[MODEL_BUFFER][0] =			memory::CommandAllocator(model_buffer_size, sizeof(ModelCommand), allocator.Alloc(model_buffer_size));
	m_CommandBuffers[NO_DEFERRED_BUFFER][0] =	memory::CommandAllocator(model_buffer_size, sizeof(ModelCommandNonDeferred), allocator.Alloc(model_buffer_size));
	m_CommandBuffers[SPOTLIGHT_BUFFER][0] =		memory::CommandAllocator(spotlight_buffer_size, sizeof(SpotlightCommand), allocator.Alloc(spotlight_buffer_size));
	m_CommandBuffers[PARTICLE_BUFFER][0] =		memory::CommandAllocator(particle_buffer_size, sizeof(ParticleCommand), allocator.Alloc(particle_buffer_size));
	m_CommandBuffers[LINE_BUFFER][0] =			memory::CommandAllocator(line_buffer_size, sizeof(LineCommand), allocator.Alloc(line_buffer_size));
	m_CommandBuffers[POINTLIGHT_BUFFER][0] =	memory::CommandAllocator(pointlight_buffer_size, sizeof(PointlightCommand), allocator.Alloc(pointlight_buffer_size));
	m_CommandBuffers[SPRITE_BUFFER][0] =		memory::CommandAllocator(sprite_buffer_size, sizeof(SpriteCommand), allocator.Alloc(sprite_buffer_size));
	m_CommandBuffers[TEXT_BUFFER][0] =			memory::CommandAllocator(text_buffer_size, sizeof(TextCommand), allocator.Alloc(text_buffer_size));

	m_CommandBuffers[MODEL_BUFFER][1] =			memory::CommandAllocator(model_buffer_size, sizeof(ModelCommand), allocator.Alloc(model_buffer_size));
	m_CommandBuffers[NO_DEFERRED_BUFFER][1] =	memory::CommandAllocator(model_buffer_size, sizeof(ModelCommandNonDeferred), allocator.Alloc(model_buffer_size));
	m_CommandBuffers[SPOTLIGHT_BUFFER][1] =		memory::CommandAllocator(spotlight_buffer_size, sizeof(SpotlightCommand), allocator.Alloc(spotlight_buffer_size));
	m_CommandBuffers[PARTICLE_BUFFER][1] =		memory::CommandAllocator(particle_buffer_size, sizeof(ParticleCommand), allocator.Alloc(particle_buffer_size));
	m_CommandBuffers[LINE_BUFFER][1] =			memory::CommandAllocator(line_buffer_size, sizeof(LineCommand), allocator.Alloc(line_buffer_size));
	m_CommandBuffers[POINTLIGHT_BUFFER][1] =	memory::CommandAllocator(pointlight_buffer_size, sizeof(PointlightCommand), allocator.Alloc(pointlight_buffer_size));
	m_CommandBuffers[SPRITE_BUFFER][1] =		memory::CommandAllocator(sprite_buffer_size, sizeof(SpriteCommand), allocator.Alloc(sprite_buffer_size));
	m_CommandBuffers[TEXT_BUFFER][1] =			memory::CommandAllocator(text_buffer_size, sizeof(TextCommand), allocator.Alloc(text_buffer_size));

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
		std::this_thread::yield();

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


void* Synchronizer::GetMemoryBlock(eBufferType buffer_type, s32 index, s32& size_of_block_out)
{
	memory::CommandAllocator& allocator = m_CommandBuffers[buffer_type][index];
	size_of_block_out = allocator.GetAllocationSize();
	return allocator.GetStart();
}

const memory::CommandAllocator& Synchronizer::GetRenderCommands(const eBufferType& buffer_type) const
{
	return m_CommandBuffers[buffer_type][m_CurrentBuffer];
}