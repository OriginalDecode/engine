#pragma once
#include <DataStructures/StaticArray.h>
#include <DataStructures/GrowingArray.h>
#include "RenderCommand.h"
#include "engine_shared.h"

#include <Engine/CommandAllocator.h>

class MemoryBlock
{
public:
	MemoryBlock() = default;
	MemoryBlock(s32 size_in_bytes, s32 alignment)
		: m_Alignment(alignment)
		, m_Size(size_in_bytes)
	{
		m_Start = malloc(size_in_bytes);
		m_CurrentPos = m_Start;
	}

	MemoryBlock(void* pStart, s32 size_in_bytes, s32 alignment)
		: m_Start(pStart)
		, m_CurrentPos(pStart)
		, m_Size(size_in_bytes)
		, m_Alignment(alignment)
	{

		m_MaxAmount = m_Size / m_Alignment;

	}

	~MemoryBlock()
	{
		m_Start = nullptr;
		m_CurrentPos = nullptr;
	}

	void CleanUp()
	{
		free(m_Start);
	}

	void* GetCurrentPos() { return m_CurrentPos; }

	void* Alloc()
	{
		m_CurrentPos = (void*)((u64)m_Start + (m_Alignment * m_CurrentSize));
		m_CurrentSize++;
		return m_CurrentPos;
	}

	void* GetStart() { return m_Start; }

	void* operator[](s32 index)
	{
		return ( void* ) ( ( u64 ) m_Start + ( m_Alignment * index ) );
	}

	void* operator[](s32 index) const
	{
		return ( void* ) ( ( u64 ) m_Start + ( m_Alignment * index ) );
	}

	void Add()
	{
		assert(m_CurrentSize < m_MaxAmount && "Tried to add too many commands to list!");

		m_CurrentPos = ( void* ) ( ( u64 ) m_CurrentPos + m_Alignment );
		m_CurrentSize++;
	}

	void Clear()
	{
		m_CurrentPos = m_Start;
		m_CurrentSize = 0;
	}

	s32 Size() const { return m_CurrentSize; }


	typedef void* iterator;
	typedef const void* const_iterator;

	iterator begin() { return m_Start; }
	const_iterator begin() const { return m_Start; }

	iterator end() { return ( void* ) ( ( u64 ) m_Start + m_CurrentSize ); }
	const_iterator end() const { return ( void* ) ( ( u64 ) m_Start + m_CurrentSize ); }

private:
	void* m_Start = nullptr;
	void* m_CurrentPos = nullptr;
	s32 m_Size = 0; //Allocated Memory
	s32 m_CurrentSize = 0; //Number of Allocations
	s32 m_MaxAmount = 0;
	s32 m_Alignment = 0; //

};

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



typedef CU::StaticArray<CommandAllocator, 2> CommandBuffer;
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

	bool LogicHasFinished() { return m_LogicDone; }
	CommandAllocator& GetAllocator(eBufferType buffer_type, s32 index);
	
	ADD_COMMAND_FUNC(eBufferType::MODEL_BUFFER,			ModelCommand);
	ADD_COMMAND_FUNC(eBufferType::MODEL_BUFFER,			ShadowCommand);
	ADD_COMMAND_FUNC(eBufferType::NO_DEFERRED_BUFFER,	ModelCommandNonDeferred);
	ADD_COMMAND_FUNC(eBufferType::SPOTLIGHT_BUFFER,		SpotlightCommand);
	ADD_COMMAND_FUNC(eBufferType::PARTICLE_BUFFER,		ParticleCommand);
	ADD_COMMAND_FUNC(eBufferType::LINE_BUFFER,			LineCommand);
	ADD_COMMAND_FUNC(eBufferType::POINTLIGHT_BUFFER,	PointlightCommand);
	ADD_COMMAND_FUNC(eBufferType::SPRITE_BUFFER,		SpriteCommand);
	ADD_COMMAND_FUNC(eBufferType::TEXT_BUFFER,			TextCommand);
	

	const CommandAllocator& GetRenderCommands(const eBufferType& buffer_type) const;
private:
	CommandBuffers m_CommandBuffers;
	volatile bool m_LogicDone = false;
	volatile bool m_RenderDone = false;
	volatile bool m_QuitFlag = false;
	u16 m_CurrentBuffer = 0;

	char* m_MainMemory = nullptr;
	CommandAllocator m_Allocator; 

};

__forceinline bool Synchronizer::HasQuit()
{
	return m_QuitFlag;
}

