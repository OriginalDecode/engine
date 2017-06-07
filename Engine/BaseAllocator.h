#pragma once
#include "engine_shared.h"

#include <assert.h>
#include <memory.h>

namespace
{
	inline void* AlignForward(void* address, u8 alignment)
	{
		return (void*)((reinterpret_cast<uptr>(address) + static_cast<uptr>(alignment - 1)) & ~(alignment - 1));
	}

	inline u8 AlignForwardAdjustment(const void* address, u8 alignment)
	{
		u8 adjustment = alignment - (reinterpret_cast<u64>(address)
			& static_cast<uptr>(alignment - 1));
		if (adjustment == alignment)
			return 0;

		return adjustment;
	}

	inline u8 AlignForwardAdjustmentWithHeader(const void* address, u8 alignment, u8 headerSize)
	{
		u8 adjustment = AlignForwardAdjustment(address, alignment);

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
	BaseAllocator() = default;
	BaseAllocator(s32 size_in_bytes, void* pStart);
	virtual void CleanUp();

	virtual void* Alloc(s32 size_in_bytes, u8 alignmnet = sizeof(void*)) = 0;
	virtual void Dealloc(void* p) = 0;

	void* GetStart() const { return m_Start; }
	void* GetCurrentPos() const { return m_CurrentPos; }
	s32 GetMemoryUsed() const { return m_UsedMemory; }
	s32 GetAllocationSize() const { return m_AllocatedMemory; }
	s32 Size() const { return m_NumberOfAllocations; }

protected:
	void* m_Start = nullptr;
	void* m_CurrentPos = nullptr;
	s32 m_AllocatedMemory = 0;
	s32 m_UsedMemory = 0;
	s32 m_NumberOfAllocations = 0;

};

