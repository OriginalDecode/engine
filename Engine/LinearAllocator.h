#pragma once
#include "BaseAllocator.h"

namespace memory
{
	class LinearAllocator : public BaseAllocator
	{
	public:
		LinearAllocator() = default;
		LinearAllocator(s32 size_in_bytes, void* start);

		void* Alloc(s32 size_in_bytes, u8 alignment = sizeof(void*)) override;
		void CleanUp() override;
		void clear();
	private:
		void Dealloc(void* pointer) override;


	};
};