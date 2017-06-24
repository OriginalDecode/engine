#include "stdafx.h"
#include "MemorySegmentHandle.h"
#include <Engine/RenderCommand.h>
#include <Engine/Engine.h>
#include <Engine/Synchronizer.h>
/*

	Allocators should be under the same namespace as the segemtn handle. Might have to update the segment handle to be able to handle several different types of segments.
	It's very clear that this class in its current state isn't very generic and does not have any further use more than to continue to use it as it is being used right now.
	Should this class be something more towards. CommandAllocatorMemorySegmentHandle CAMS

*/
namespace memory
{
	void MemorySegmentHandle::Initiate(/*takes a memory_block, size, and offset */)
	{
		s32 allocation_size0 = 0;
		void* memory_block0 = Engine::GetInstance()->GetSynchronizer()->GetMemoryBlock(eBufferType::MODEL_BUFFER, 0, allocation_size0);
		s32 allocation_size1 = 0;
		void* memory_block1 = Engine::GetInstance()->GetSynchronizer()->GetMemoryBlock(eBufferType::MODEL_BUFFER, 1, allocation_size1);

		DL_ASSERT_EXP(allocation_size0 == allocation_size1, "different sizes for double buffer?");

		LinearAllocator alloc0 = LinearAllocator(allocation_size0, memory_block0);
		LinearAllocator alloc1 = LinearAllocator(allocation_size1, memory_block1);


		s32 segment_size = allocation_size1 / 8;

		for (s32 i = 0; i < 8; i++)
		{
			m_Segments[0][i] = CommandAllocator(segment_size, sizeof(ModelCommand), alloc0.Alloc(segment_size));
			m_Segments[1][i] = CommandAllocator(segment_size, sizeof(ModelCommand), alloc1.Alloc(segment_size));
		}
	}

	CommandAllocator& MemorySegmentHandle::GetCommandAllocator(s32 buffer, s32 index)
	{
		return m_Segments[buffer][index];
	}

	void MemorySegmentHandle::Clear(s32 index)
	{
		for (s32 i = 0; i < 8; i++)
		{
			m_Segments[index][i].Clear();
		}
	}
};