#pragma once
#include <Engine/BaseAllocator.h>

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

private:
	void Dealloc(void*) override { assert(false && "Use Clear()"); }
	s32 m_Offset = 0;
};

