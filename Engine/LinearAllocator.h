#pragma once
#include "BaseAllocator.h"

class LinearAllocator : public BaseAllocator
{
public:
	LinearAllocator(s32 size_in_bytes, void* start);
	LinearAllocator();

	void Initiate(s32 size_in_bytes, void * pStart);
	void* alloc(s32 size_in_bytes, u8 alignment = sizeof(void*)) override;
	void dealloc(void* pointer) override;
	bool CleanUp() override;
	void clear();
private:

	LinearAllocator(const LinearAllocator&) = delete;
	LinearAllocator& operator=(const LinearAllocator&) = delete;


};

