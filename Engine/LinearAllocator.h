#pragma once
#include "BaseAllocator.h"

namespace memory
{
	class LinearAllocator : public BaseAllocator
	{
	public:
		LinearAllocator() = default;
		LinearAllocator(int32 size_in_bytes, void* start);

		void* Alloc(int32 size_in_bytes, uint8 alignment = sizeof(void*)) override;
		void CleanUp() override;
		void clear();
	private:
		void Dealloc(void* pointer) override;


	};
};