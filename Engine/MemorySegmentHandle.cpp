#include "stdafx.h"
#include "MemorySegmentHandle.h"
#include <Engine/RenderCommand.h>
#include <Engine/Engine.h>
#include <Engine/Synchronizer.h>

namespace memory
{
	void MemorySegmentHandle::Initiate(/*takes a memory_block, size, and offset */)
	{
		//s32 allocation_size0 = 0;
		//void* memory_block0 = Engine::GetInstance()->GetSynchronizer()->GetMemoryBlock(eBufferType::MODEL_BUFFER, 0, allocation_size0);
		//s32 allocation_size1 = 0;
		//void* memory_block1 = Engine::GetInstance()->GetSynchronizer()->GetMemoryBlock(eBufferType::MODEL_BUFFER, 1, allocation_size1);

		//ASSERT(allocation_size0 == allocation_size1, "different sizes for double buffer?");

		//LinearAllocator alloc0 = LinearAllocator(allocation_size0, memory_block0);
		//LinearAllocator alloc1 = LinearAllocator(allocation_size1, memory_block1);


		//s32 segment_size = allocation_size1 / 8;
		//constexpr s32 commands_per_segment = 3000 / 8;
		//for (s32 i = 0; i < 8; i++)
		//{
		//	m_Segments[0][i] = CommandAllocator(segment_size, sizeof(ModelCommand), alloc0.Alloc(segment_size));
		//	m_Segments[1][i] = CommandAllocator(segment_size, sizeof(ModelCommand), alloc1.Alloc(segment_size));

		///*	for (s32 j = 0; j < commands_per_segment; j++)
		//	{
		//		m_Segments[0][i].Alloc(sizeof(ModelCommand));
		//		m_Segments[1][i].Alloc(sizeof(ModelCommand));
		//	}
		//	m_Segments[0][i].Clear();
		//	m_Segments[1][i].Clear();*/
		//}

	


	}

	CommandAllocator& MemorySegmentHandle::GetCommandAllocator(int32 buffer, int32 index)
	{
		return m_Segments[buffer][index];
	}

	const memory::CommandAllocator& MemorySegmentHandle::GetCommandAllocator(int32 buffer, int32 index) const
	{
		return m_Segments[buffer][index];
	}

	void MemorySegmentHandle::Clear(int32 index)
	{
		for (int32 i = 0; i < 8; i++)
		{
			m_Segments[index][i].Clear();
		}
	}

	int32 MemorySegmentHandle::CommandSize(int32 index)
	{
		int32 total = 0;
		for (int32 i = 0; i < 8; i++)
		{
			total += m_Segments[index][i].Size();
		}
		return total;
	}

};