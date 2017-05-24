#pragma once
#include <DataStructures/StaticArray.h>
#include <DataStructures/GrowingArray.h>
#include "RenderCommand.h"
#include "snowblind_shared.h"

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
	s32 m_Size = 0;
	s32 m_CurrentSize = 0;
	s32 m_Alignment = 0;

};

enum eBufferType
{
	MODEL_BUFFER,
	SPOTLIGHT_BUFFER,
	PARTICLE_BUFFER,
	LINE_BUFFER,
	POINTLIGHT_BUFFER,
	SPRITE_BUFFER,
	TEXT_BUFFER,
	BUFFER_COUNT
};

#define ADD_COMMAND_FUNC(buffer_type, command_type)\
void AddRenderCommand(const command_type& command)\
{\
	DL_PRINT("Adding %s to %s", #command_type, #buffer_type);\
	CommandBuffer& buffer = myCommandBuffers[buffer_type];\
	DL_PRINT("Buffer %s received", #buffer_type);\
	void* current = buffer[m_CurrentBuffer ^ 1].GetCurrentPos();\
	DL_PRINT("starting memcpy from command to CurrentPos()");\
	memcpy(current, &command, sizeof(command_type));\
	DL_PRINT("memcpy successfully ended");\
	buffer[m_CurrentBuffer ^ 1].Add();\
	DL_PRINT("moving pointer to next portion");\
};



typedef CU::StaticArray<MemoryBlock, 2> CommandBuffer;
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

	bool LogicHasFinished() { return myLogicIsDone; }

	template<typename T>
	void AddRenderCommand(T& command, eBufferType buffer_type)
	{
		CommandBuffer& buffer = myCommandBuffers[( u32 ) buffer_type];
		void* current = ( buffer[m_CurrentBuffer ^ 1].GetCurrentPos() );
		memcpy(current, &command, sizeof(T));
		buffer[m_CurrentBuffer ^ 1].Add();
	}

	ADD_COMMAND_FUNC(eBufferType::MODEL_BUFFER,			ModelCommand);
	ADD_COMMAND_FUNC(eBufferType::SPOTLIGHT_BUFFER,		SpotlightCommand);
	ADD_COMMAND_FUNC(eBufferType::PARTICLE_BUFFER,		ParticleCommand);
	ADD_COMMAND_FUNC(eBufferType::LINE_BUFFER,			LineCommand);
	ADD_COMMAND_FUNC(eBufferType::POINTLIGHT_BUFFER,	PointlightCommand);
	ADD_COMMAND_FUNC(eBufferType::SPRITE_BUFFER,		SpriteCommand);
	ADD_COMMAND_FUNC(eBufferType::TEXT_BUFFER,			TextCommand);

	const MemoryBlock& GetRenderCommands(const eBufferType& buffer_type) const;
private:
	CommandBuffers myCommandBuffers;
	volatile bool myLogicIsDone;
	volatile bool myRenderIsDone;
	volatile bool myQuitFlag;
	u16 m_CurrentBuffer;

	void* m_MainMemory = nullptr;
	s32 m_MemorySize = 0;
	s32 m_AllocationAmt = 0;

	MemoryBlock m_MainBlock;


	template<typename T>
	void RegisterBuffer(eBufferType buffer_type);

};

__forceinline bool Synchronizer::HasQuit()
{
	return myQuitFlag;
}

template<typename T>
void Synchronizer::RegisterBuffer(eBufferType buffer_type)
{
	myCommandBuffers[buffer_type][0] = MemoryBlock(m_MainBlock.Alloc(), m_AllocationAmt, sizeof(T));
	myCommandBuffers[buffer_type][1] = MemoryBlock(m_MainBlock.Alloc(), m_AllocationAmt, sizeof(T));
}

