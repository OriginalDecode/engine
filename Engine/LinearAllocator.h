#pragma once
#include "BaseAllocator.h"
class LinearAllocator : public BaseAllocator
{
public:
	LinearAllocator() = default;

	void Initiate(u64 size_in_bytes) override;
	void CleanUp() override;
	void* Alloc(u64 size_in_bytes) override;


	void Dealloc(void* pMemroy) override;

	void Clear();
private:

};
