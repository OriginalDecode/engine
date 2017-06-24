#include "stdafx.h"
#include "BaseAllocator.h"

namespace memory
{
	BaseAllocator::BaseAllocator(s32 size_in_bytes, void* pStart)
		: m_Start(pStart)
		, m_CurrentPos(pStart)
		, m_AllocatedMemory(size_in_bytes)
	{
	}

	void BaseAllocator::CleanUp()
	{
		m_Start = nullptr;
		m_AllocatedMemory = 0;
	}
};