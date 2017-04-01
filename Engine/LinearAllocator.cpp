#include "stdafx.h"
#include "LinearAllocator.h"

LinearAllocator::LinearAllocator(s32 size_in_bytes, void* start)
	: BaseAllocator(size_in_bytes, start)
{
	assert(size_in_bytes > 0 && "Can't allocate 0 or less bytes.");
}

LinearAllocator::LinearAllocator()
	:BaseAllocator(0, nullptr)
{
}

void LinearAllocator::Initiate(s32 size_in_bytes, void * pStart)
{
	m_AllocatedMemory = size_in_bytes;
	m_Start = pStart;
	m_CurrentPos = pStart;
}

void* LinearAllocator::alloc(s32 size_in_bytes, u8 alignment)
{
	assert(size_in_bytes > 0 && "Can't allocate 0 or less bytes!");
	assert(alignment > 0 && "It's recommended to use an alignment that is larger than 0 due to higher memory usage.");
	u8 adjustment = alignForwardAdjustment(m_CurrentPos, alignment);
	assert(m_UsedMemory + adjustment + size_in_bytes <= m_AllocatedMemory && "Out of memory!");

	u64 aligned_address = (u64)m_CurrentPos + adjustment;
	m_CurrentPos = (void*)(aligned_address + size_in_bytes);
	m_UsedMemory += size_in_bytes + adjustment;
	m_NumberOfAllocations++;

	return (void*)aligned_address;
}

void LinearAllocator::dealloc(void* /*pointer*/)
{
	assert(false && "Use clear()");
}

bool LinearAllocator::CleanUp()
{
	m_CurrentPos = nullptr;
	__super::CleanUp();
	return true;
}

void LinearAllocator::clear()
{
	m_NumberOfAllocations = 0;
	m_UsedMemory = 0;
	m_CurrentPos = m_Start;
}

