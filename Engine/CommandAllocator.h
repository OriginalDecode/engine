#pragma once
#include <Engine/BaseAllocator.h>

namespace memory
{
	class CommandAllocator : public BaseAllocator
	{
	public:
		CommandAllocator() = default;
		CommandAllocator(int32 size_in_bytes, void* pStart);
		CommandAllocator(int32 size_in_bytes, int32 offset_in_bytes, void* pStart);
		void CleanUp() override;

		void* operator[](int32 index);
		void* operator[](int32 index) const;

		void* Alloc(int32 size_in_bytes, uint8 alignment = sizeof(void*)) override;
		void Clear();

	private:
		void Dealloc(void*) override { assert(false && "Use Clear()"); }
		int32 m_Offset = 0;
	};
};