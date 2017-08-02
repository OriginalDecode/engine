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


	//ADD_COMMAND_FUNC(eBufferType::MODEL_BUFFER, ModelCommand);


	void AddRenderCommand(const ModelCommand command)
	{
		if (m_QuitFlag)
			return;
		CommandBuffer& buffer = m_CommandBuffers[eBufferType::MODEL_BUFFER];
		void* current = buffer[m_CurrentBuffer ^ 1].Alloc(sizeof(ModelCommand));
		memcpy(current, &command, sizeof(ModelCommand));
	};

	ADD_COMMAND_FUNC(eBufferType::NO_DEFERRED_BUFFER, ModelCommandNonDeferred);
	ADD_COMMAND_FUNC(eBufferType::SPOTLIGHT_BUFFER, SpotlightCommand);
	ADD_COMMAND_FUNC(eBufferType::PARTICLE_BUFFER, ParticleCommand);
	ADD_COMMAND_FUNC(eBufferType::LINE_BUFFER, LineCommand);
	ADD_COMMAND_FUNC(eBufferType::POINTLIGHT_BUFFER, PointlightCommand);
	ADD_COMMAND_FUNC(eBufferType::SPRITE_BUFFER, SpriteCommand);
	ADD_COMMAND_FUNC(eBufferType::TEXT_BUFFER, TextCommand);


	const memory::CommandAllocator& GetRenderCommands(const eBufferType& buffer_type) const;
private:
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

