#include "stdafx.h"
#include "CommandAllocator.h"
namespace memory
{
	CommandAllocator::CommandAllocator(s32 size_in_bytes, s32 offset_in_bytes, void* pStart)
		: BaseAllocator(size_in_bytes, pStart)
	{
		/* Adjust the offset to concider memory alignment */
		u64 aligned_address = (u64)m_CurrentPos + offset_in_bytes;
		void* adjust_to = (void*)aligned_address;
		u8 adjustment = AlignForwardAdjustment(adjust_to, sizeof(void*));
		m_Offset = offset_in_bytes + adjustment;


	}

	CommandAllocator::CommandAllocator(s32 size_in_bytes, void* pStart)
		: BaseAllocator(size_in_bytes, pStart)
	{
	}

	void* CommandAllocator::Alloc(s32 size_in_bytes, u8 alignment /*= sizeof(void*)*/)
	{
		AtomicAddPtr((void**)&m_CurrentPos, size_in_bytes);
		void* ptr0 = AtomicAddPtr((void**)&m_CurrentPos, size_in_bytes);
		AtomicAddU32(&m_UsedMemory, size_in_bytes);
		AtomicAddS32(&m_NumberOfAllocations, 1);

		return ptr0;
		



	/*	assert(size_in_bytes > 0 && "Can't allocate 0 or less bytes!");
		assert(alignment > 0 && "It's recommended to use an alignment that is larger than 0 due to higher memory usage.");
		u8 adjustment = AlignForwardAdjustment(m_CurrentPos, alignment);
		assert(m_UsedMemory + adjustment + size_in_bytes <= m_AllocatedMemory && "Out of memory!");

		u64 aligned_address = (u64)m_CurrentPos + adjustment;
		m_CurrentPos = (void*)(aligned_address + size_in_bytes);
		m_UsedMemory += size_in_bytes + adjustment;
		m_NumberOfAllocations++;

		return (void*)aligned_address;*/

	}

	void* CommandAllocator::operator[](s32 index)
	{
		return (void*)((u64)m_Start + (m_Offset * index));
	}

	void* CommandAllocator::operator[](s32 index) const
	{
		return (void*)((u64)m_Start + (m_Offset * index));
	}


	void CommandAllocator::CleanUp()
	{
		BaseAllocator::CleanUp();
	}

	void CommandAllocator::Clear()
	{
		m_CurrentPos = m_Start;
		m_UsedMemory = 0;
		m_NumberOfAllocations = 0;

	}
};