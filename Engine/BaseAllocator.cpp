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
	assert(m_NumberOfAllocations == 0 && m_UsedMemory == 0 && "Memory leak detected!");
	m_Start = nullptr;
	m_AllocatedMemory = 0;
}
