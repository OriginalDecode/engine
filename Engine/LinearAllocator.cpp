#include "stdafx.h"
#include "LinearAllocator.h"

namespace memory
{
	LinearAllocator::LinearAllocator(int32 size_in_bytes, void* start)
		: BaseAllocator(size_in_bytes, start)
	{
		assert(size_in_bytes > 0 && "Can't allocate 0 bytes");
	}

	void* LinearAllocator::Alloc(int32 size_in_bytes, uint8 alignment)
	{
		assert(size_in_bytes > 0 && "Can't allocate 0 or less bytes!");
		assert(alignment > 0 && "It's recommended to use an alignment that is larger than 0 due to higher memory usage.");
		uint8 adjustment = AlignForwardAdjustment(m_CurrentPos, alignment);
		assert(m_UsedMemory + adjustment + size_in_bytes <= m_AllocatedMemory && "Out of memory!");

		uint64 aligned_address = (uint64)m_CurrentPos + adjustment;
		m_CurrentPos = (void*)(aligned_address + size_in_bytes);
		m_UsedMemory += size_in_bytes + adjustment;
		m_NumberOfAllocations++;

		return (void*)aligned_address;
	}

	void LinearAllocator::Dealloc(void* /*pointer*/)
	{
		assert(false && "Use clear()");
	}

	void LinearAllocator::CleanUp()
	{
		m_CurrentPos = nullptr;
		BaseAllocator::CleanUp();
	}

	void LinearAllocator::clear()
	{
		m_NumberOfAllocations = 0;
		m_UsedMemory = 0;
		m_CurrentPos = m_Start;
	}
};
