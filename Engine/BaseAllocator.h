#pragma once
#include "engine_shared.h"

#include <assert.h>
#include <memory.h>

namespace
{
	inline void* AlignForward(void* address, uint8 alignment)
	{
		return (void*)((reinterpret_cast<uptr>(address) + static_cast<uptr>(alignment - 1)) & ~(alignment - 1));
	}

	inline uint8 AlignForwardAdjustment(const void* address, uint8 alignment)
	{
		uint8 adjustment = alignment - (reinterpret_cast<uptr>(address) & static_cast<uptr>(alignment - 1));
		if (adjustment == alignment)
			return 0;

		return adjustment;
	}

	inline uint8 AlignForwardAdjustmentWithHeader(const void* address, uint8 alignment, uint8 headerSize)
	{
		uint8 adjustment = AlignForwardAdjustment(address, alignment);

		uint8 neededSpace = headerSize;

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

namespace memory
{
	class BaseAllocator
	{
	public:
		BaseAllocator() = default;
		BaseAllocator(int32 size_in_bytes, void* pStart);
		virtual void CleanUp();

		virtual void* Alloc(int32 size_in_bytes, uint8 alignmnet = sizeof(void*)) = 0;
		virtual void Dealloc(void* p) = 0;

		void* GetStart() const { return m_Start; }
		void* GetCurrentPos() const { return m_CurrentPos; }
		int32 GetMemoryUsed() const { return m_UsedMemory; }
		int32 GetAllocationSize() const { return m_AllocatedMemory; }
		int32 Size() const { return m_NumberOfAllocations; }


		static void* Alloc(int32 size_in_bytes, void* block, uint8 alignment = sizeof(void*));

	protected:
		void* m_Start = nullptr;
		void* m_CurrentPos = nullptr;
		int32 m_AllocatedMemory = 0;
		int32 m_UsedMemory = 0;
		int32 m_NumberOfAllocations = 0;

	};
};