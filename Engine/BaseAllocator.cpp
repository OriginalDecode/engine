#include "stdafx.h"
#include "BaseAllocator.h"

BaseAllocator::BaseAllocator(s32 size_in_bytes, void* pStart)
	: m_Start(pStart)
	, m_CurrentPos(pStart)
	, m_AllocatedMemory(size_in_bytes)
{
}

void BaseAllocator::CleanUp()
{
	m_Start = nullptr;
	m_AllocatedMemory = 0;
}

//void* BaseAllocator::Alloc(s32 size_in_bytes, void* block, u8 alignment /*= sizeof(void*)*/)
//{
//	assert(size_in_bytes > 0 && "Can't allocate 0 or less bytes!");
//	assert(alignment > 0 && "It's recommended to use an alignment that is larger than 0 due to higher memory usage.");
//	u8 adjustment = AlignForwardAdjustment(m_CurrentPos, alignment);
//	assert(m_UsedMemory + adjustment + size_in_bytes <= m_AllocatedMemory && "Out of memory!");
//
//	u64 aligned_address = (u64)m_CurrentPos + adjustment;
//	m_CurrentPos = (void*)(aligned_address + size_in_bytes);
//	m_UsedMemory += size_in_bytes + adjustment;
//	m_NumberOfAllocations++;
//
//	return (void*)aligned_address;
//}
