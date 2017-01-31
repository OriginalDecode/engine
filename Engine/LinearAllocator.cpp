#include "stdafx.h"
#include "LinearAllocator.h"

void LinearAllocator::Initiate(u64 size_in_bytes)
{
	m_Memory = malloc(size_in_bytes);
	m_AllocatedMemory = size_in_bytes;
}

void LinearAllocator::CleanUp()
{
	free(m_Memory);
	m_AllocatedMemory = 0;
	m_UsedMemory = 0;
	//debug message?
}

void* LinearAllocator::Alloc(u64 size_in_bytes)
{
	assert(m_UsedMemory + size_in_bytes < m_AllocatedMemory && "Tried to allocate outside designated memory block!");
	m_UsedMemory += size_in_bytes;
	m_NumAllocations++;

	u64 uptr = (u64)m_Memory;
	uptr += size_in_bytes;
	m_Memory = (void*)uptr;
	return m_Memory;
}

void LinearAllocator::Dealloc(void* pMemroy)
{
	assert(false && "use clear!");
}

void LinearAllocator::Clear()
{
	u64 uptr = (u64)m_Memory;
	uptr -= m_UsedMemory;
	m_Memory = (void*)uptr;
}
