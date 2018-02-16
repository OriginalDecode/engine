#pragma once
#include <DataStructures/StaticArray.h>
#include <DataStructures/GrowingArray.h>
#include "RenderCommand.h"
#include "engine_shared.h"

#include <Engine/CommandAllocator.h>

enum eBufferType
{
	MODEL_BUFFER,
	NO_DEFERRED_BUFFER,
	SPOTLIGHT_BUFFER,
	PARTICLE_BUFFER,
	LINE_BUFFER,
	POINTLIGHT_BUFFER,
	SPRITE_BUFFER,
	TEXT_BUFFER,
	BUFFER_COUNT
};

#define ADD_COMMAND_FUNC(buffer_type, command_type)\
void AddRenderCommand(const command_type command)\
{\
	if(m_QuitFlag)\
		return;\
	CommandBuffer& buffer = m_CommandBuffers[buffer_type];\
	void* current = buffer[m_CurrentBuffer ^ 1].Alloc(sizeof(command_type));\
	memcpy(current, &command, sizeof(command_type));\
};

typedef CU::StaticArray<memory::CommandAllocator, 2> CommandBuffer;
typedef CU::StaticArray<CommandBuffer, eBufferType::BUFFER_COUNT> CommandBuffers;

class Synchronizer
{
public:
	Synchronizer() = default;
	~Synchronizer();
	bool Initiate();
	void SwapBuffer();
	void Quit();
	bool HasQuit();
	void WaitForRender();
	void WaitForLogic();
	void RenderIsDone();
	void LogicIsDone();
	u16 GetCurrentBufferIndex() const { return m_CurrentBuffer; }

	bool LogicHasFinished() { return m_LogicDone; }

	void* GetMemoryBlock(eBufferType buffer_type, s32 index, s32& size_of_block_out);

	void AddRenderCommand(const ModelCommand& render_command);
	void AddRenderCommand(const ModelCommandNonDeferred& render_command);
	void AddRenderCommand(const SpotlightCommand& render_command);
	void AddRenderCommand(const ParticleCommand& render_command);
	void AddRenderCommand(const LineCommand& render_command);
	void AddRenderCommand(const PointlightCommand& render_command);
	void AddRenderCommand(const SpriteCommand& render_command);
	void AddRenderCommand(const TextCommand& render_command);

	const memory::CommandAllocator& GetRenderCommands(const eBufferType& buffer_type) const;
	const memory::CommandAllocator& GetLineBuffer() const { return GetRenderCommands(eBufferType::LINE_BUFFER); }
	const memory::CommandAllocator& GetModelBuffer() const { return GetRenderCommands(eBufferType::MODEL_BUFFER); }
	const memory::CommandAllocator& GetSpotlightBuffer() const { return GetRenderCommands(eBufferType::SPOTLIGHT_BUFFER); }
	const memory::CommandAllocator& GetPointlightBuffer() const { return GetRenderCommands(eBufferType::POINTLIGHT_BUFFER);	}
	const memory::CommandAllocator& GetTextBuffer() const { return GetRenderCommands(eBufferType::TEXT_BUFFER); }
	const memory::CommandAllocator& GetSpriteBuffer() const { return GetRenderCommands(eBufferType::SPRITE_BUFFER); }
	const memory::CommandAllocator& GetParticleBuffer() const { return GetRenderCommands(eBufferType::PARTICLE_BUFFER); }
	const memory::CommandAllocator& GetNonDeferredBuffer() const { return GetRenderCommands(eBufferType::NO_DEFERRED_BUFFER); }

private:
	template<typename command>
	void AddRenderCommand(eBufferType command_buffer_type, const command& render_command);

	CommandBuffers m_CommandBuffers;
	volatile bool m_LogicDone = false;
	volatile bool m_RenderDone = false;
	volatile bool m_QuitFlag = false;
	u16 m_CurrentBuffer = 0;

	void* m_MainMemory = nullptr;

};

__forceinline bool Synchronizer::HasQuit()
{
	return m_QuitFlag;
}

template<typename command>
void Synchronizer::AddRenderCommand(eBufferType command_buffer_type, const command& render_command)
{
	if (m_QuitFlag)
		return;

	CommandBuffer& buffer = m_CommandBuffers[command_buffer_type];
	void* current = buffer[m_CurrentBuffer ^ 1].Alloc(sizeof(command));
	memcpy(current, &render_command, sizeof(command));
}
