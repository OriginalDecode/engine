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

	~MemoryBlock()
	{
		free(m_Start);
		m_Start = nullptr;
		m_CurrentPos = nullptr;
	}


	void* GetCurrentPos() { return m_CurrentPos; }
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

typedef CU::StaticArray<MemoryBlock, 2> CommandBuffer;
typedef CU::StaticArray<CommandBuffer, (u32)eCommandBuffer::_COUNT> CommandBuffers;






class Synchronizer
{
public:
	Synchronizer() = default;
	bool Initiate();
	void SwapBuffer();
	void Clear();
	void Quit();
	bool HasQuit();
	void WaitForRender();
	void WaitForLogic();
	void RenderIsDone();
	void LogicIsDone();

	bool LogicHasFinished() { return myLogicIsDone; }

	//void AddRenderCommand(const RenderCommand& aRenderCommand);
	bool ValidateBuffer(eCommandBuffer buffer_type, RenderCommand2::eCommandType command_type)
	{
		if ( command_type == RenderCommand2::MODEL )
		{
			if ( buffer_type == eCommandBuffer::e3D )
				return true;

			return false;
		}

		if ( command_type == RenderCommand2::SPOTLIGHT )
		{
			if ( buffer_type == eCommandBuffer::eSpotlight )
				return true;
			return false;
		}

		return false;
	}
	template<typename T>
	void AddRenderCommand(T& command, eCommandBuffer buffer_type)
	{
		RenderCommand2& c = command;
		const bool result = ValidateBuffer(buffer_type, c.m_CommandType);
		DL_ASSERT_EXP(result, "Incorrect Buffer used for this CommandType");

		CommandBuffer& buffer = myCommandBuffers[( u32 ) buffer_type];
		void* current = ( buffer[m_CurrentBuffer ^ 1].GetCurrentPos() );
		memcpy(current, &command, sizeof(T));
		buffer[m_CurrentBuffer ^ 1].Add();
	}

	const MemoryBlock& GetRenderCommands(const eCommandBuffer& buffer_type) const;
	//const CU::GrowingArray<RenderCommand>& GetRenderCommands(const eCommandBuffer& commandType) const;
private:
	CommandBuffers myCommandBuffers;
	volatile bool myLogicIsDone;
	volatile bool myRenderIsDone;
	volatile bool myQuitFlag;
	u16 m_CurrentBuffer;

};

__forceinline bool Synchronizer::HasQuit()
{
	return myQuitFlag;
}

