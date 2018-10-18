#include "stdafx.h"
#include "Synchronizer.h"
#include <thread>
#include "profile_defines.h"

Synchronizer::~Synchronizer()
{
}

bool Synchronizer::Initiate()
{
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
	m_LogicDone = true;
	m_RenderDone = true;
	m_QuitFlag = true;
}

void Synchronizer::WaitForRender()
{
	PROFILE_FUNCTION(profiler::colors::Amber);
	while (!m_RenderDone)
		std::this_thread::sleep_for(std::chrono::microseconds(1));

	m_RenderDone = false;
}

void Synchronizer::WaitForLogic()
{
	PROFILE_FUNCTION(profiler::colors::Amber100);
	while (!m_LogicDone)
		std::this_thread::sleep_for(std::chrono::microseconds(1));

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

void Synchronizer::AddRenderCommand(const ModelCommand& render_command)
{
	AddRenderCommand(eBufferType::MODEL_BUFFER, render_command);
}

void Synchronizer::AddRenderCommand(const ModelCommandNonDeferred& render_command)
{
	AddRenderCommand(eBufferType::NO_DEFERRED_BUFFER, render_command);
}

void Synchronizer::AddRenderCommand(const SpotlightCommand& render_command)
{
	AddRenderCommand(eBufferType::SPOTLIGHT_BUFFER, render_command);
}

void Synchronizer::AddRenderCommand(const ParticleCommand& render_command)
{
	AddRenderCommand(eBufferType::PARTICLE_BUFFER, render_command);
}

void Synchronizer::AddRenderCommand(const LineCommand& render_command)
{
	AddRenderCommand(eBufferType::LINE_BUFFER, render_command);
}

void Synchronizer::AddRenderCommand(const SpriteCommand& render_command)
{
	AddRenderCommand(eBufferType::SPRITE_BUFFER, render_command);
}

void Synchronizer::AddRenderCommand(const TextCommand& render_command)
{
	AddRenderCommand(eBufferType::TEXT_BUFFER, render_command);
}

void Synchronizer::AddRenderCommand(const PointlightCommand& render_command)
{
	AddRenderCommand(eBufferType::POINTLIGHT_BUFFER, render_command);
}
