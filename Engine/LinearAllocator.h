#pragma once
#include "BaseAllocator.h"
class LinearAllocator : public BaseAllocator
{
public:
	LinearAllocator() = default;

	bool CleanUp() override;
	void* alloc(s32 size_in_bytes, u8 alignmnet = sizeof(void*)) override;
	void dealloc(void* pMemroy) override;



	void Clear();
private:

};
