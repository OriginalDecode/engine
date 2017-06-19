#pragma once
#include <Engine/engine_shared.h>
#include <Engine/CommandAllocator.h>
#include <CommonLib/DataStructures/StaticArray.h>
class MemorySegmentHandle
{
public:
	MemorySegmentHandle() = default;

	void Initiate();
	CommandAllocator& GetCommandAllocator(s32 buffer, s32 index);
	void Clear(s32 index);
private:
	CU::StaticArray<CommandAllocator, 8> m_Segments[2];



};

