#pragma once
#include "snowblind_shared.h"

#include <assert.h>
#include <memory.h>

#ifndef _WIN64
typedef u32 uptr;
#else
typedef u64 uptr;
#endif

namespace
{
	inline void* alignForward(void* address, u8 alignment)
	{
		return (void*)((reinterpret_cast<uptr>(address) + static_cast<uptr>(alignment - 1)) & ~(alignment - 1));
	}

	inline u8 alignForwardAdjustment(const void* address, u8 alignment)
	{
		u8 adjustment = alignment - (reinterpret_cast<u64>(address)
			& static_cast<uptr>(alignment - 1));
		if (adjustment == alignment)
			return 0;

		return adjustment;
	}

	inline u8 alignForwardAdjustmentWithHeader(const void* address, u8 alignment, u8 headerSize)
	{
		u8 adjustment = alignForwardAdjustment(address, alignment);

		u8 neededSpace = headerSize;

		if (adjustment < neededSpace)
		{
			neededSpace -= adjustment;

			//Increase adjustment to fit header
			adjustment += alignment * (neededSpace / alignment);

			if (neededSpace % alignment > 0)
				adjustment += alignment;
		}

		return adjustment;
	}
};

class BaseAllocator
{
public:
	BaseAllocator(s32 size_in_bytes, void* pStart);
	virtual bool CleanUp();

	virtual void* alloc(s32 size_in_bytes, u8 alignmnet = sizeof(void*)) = 0;
	virtual void dealloc(void* p) = 0;

	void* GetStart() const { return m_Start; }
	void* GetCurrentPos() const { return m_CurrentPos; }
	s32 GetMemoryUsed() const { return m_UsedMemory; }
	s32 GetAllocationSize() const { return m_AllocatedMemory; }
	s32 GetNumberOfAllocations() const { return m_NumberOfAllocations; }

protected:
	void* m_Start = nullptr;
	void* m_CurrentPos = nullptr;
	s32 m_AllocatedMemory = 0;
	s32 m_UsedMemory = 0;
	s32 m_NumberOfAllocations = 0;

};

