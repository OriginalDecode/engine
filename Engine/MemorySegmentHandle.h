#pragma once
#include <Engine/engine_shared.h>
#include <Engine/CommandAllocator.h>
#include <CommonLib/DataStructures/StaticArray.h>

namespace memory
{
	class MemorySegmentHandle
	{
	public:
		MemorySegmentHandle() = default;

		void Initiate();
		CommandAllocator& GetCommandAllocator(s32 buffer, s32 index);
		const CommandAllocator& GetCommandAllocator(s32 buffer, s32 index) const;
		void Clear(s32 index);

		s32 CommandSize(s32 index);

	private:
		CU::StaticArray<CommandAllocator, 8> m_Segments[2];

	};
};