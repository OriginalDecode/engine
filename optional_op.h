#include <new>

static int allocations = 0;

void* operator new (size_t n)
{
	allocations++;
	return malloc(n);
}

void* operator new[] (size_t n)
{
	allocations++;
	return malloc(n);
}

void operator delete(void* p)
{
	allocations--;
	free(p);
}

void operator delete[](void* p)
{
	allocations--;
	free(p);
}
