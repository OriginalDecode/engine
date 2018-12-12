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
		CommandAllocator& GetCommandAllocator(int32 buffer, int32 index);
		const CommandAllocator& GetCommandAllocator(int32 buffer, int32 index) const;
		void Clear(int32 index);

		int32 CommandSize(int32 index);

	private:
		CU::StaticArray<CommandAllocator, 8> m_Segments[2];

	};
};