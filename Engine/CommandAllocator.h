#pragma once
#include <Engine/BaseAllocator.h>

namespace memory
{
	class CommandAllocator : public BaseAllocator
	{
	public:
		CommandAllocator() = default;
		CommandAllocator(s32 size_in_bytes, void* pStart);
		CommandAllocator(s32 size_in_bytes, s32 offset_in_bytes, void* pStart);
		void CleanUp() override;

		void* operator[](s32 index);
		void* operator[](s32 index) const;

		void* Alloc(s32 size_in_bytes, u8 alignment = sizeof(void*)) override;
		void Clear();


		typedef void* iterator;
		typedef const void* const_iterator;
		iterator begin() { return m_Start; }
		const_iterator begin() const { return m_Start; }
		iterator end() { return m_CurrentPos; }
		const_iterator end() const { return m_CurrentPos; }

	private:
		void Dealloc(void*) override { assert(false && "Use Clear()"); }
		s32 m_Offset = 0;
	};
};