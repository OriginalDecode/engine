#pragma once
#include <assert.h>
#include "snowblind_shared.h"
void * operator new(size_t size);
class BaseAllocator
{
public:
	BaseAllocator() = default;
		
	virtual void Initiate(u64 size_in_bytes) = 0;
	virtual void CleanUp() = 0;

	virtual void* Alloc(u64 size_in_bytes) = 0;
	virtual void Dealloc(void* pMemory) = 0;
	
	u32 GetAllocatedMemory() const { return m_AllocatedMemory; }
	u32 GetUsedMemory() const { return m_UsedMemory; }
	u32 GetNumAllocations() const { return m_NumAllocations; }
protected:

	void* m_Memory = nullptr;
	u32 m_UsedMemory = 0;
	u32 m_AllocatedMemory = 0;
	u32 m_NumAllocations = 0;
};

