//
//
//static int allocations = 0;
//
//
//
//void record_alloc(void* p, const char* file, size_t line)
//{
//	char temp[100];
//	sprintf_s(temp, "\nAllocation\n%x\n%s\n%ld\n", p, file, line);
//	OutputDebugString(temp);
//}
//
//void unrecord_alloc(void* p)
//{
//	char temp[100];
//	sprintf_s(temp , "\nDealloc\n%x\n", p);
//	OutputDebugString(temp);
//}
//
//
//char* __file__ = "unknown";
//size_t __line__ = 0;
//
//
//void* operator new(size_t n)
//{
//	allocations++;
//	void * p = malloc(n);
//	record_alloc(p, __file__, __line__);
//	__file__ = "unknown";
//	__line__ = 0;
//
//	return p;
//}
//
//void* operator new[] (size_t n)
//{
//	allocations++;
//	void * p = malloc(n);
//	record_alloc(p, __file__, __line__);
//	__file__ = "unknown";
//	__line__ = 0;
//
//	return p;
//}
//
//void operator delete(void* p)
//{
//	allocations--;
//	unrecord_alloc(p);
//	free(p);
//}
//
//void operator delete[](void* p)
//{
//	allocations--;
//	unrecord_alloc(p);
//	free(p);
//}
//
//#define new (__file__=__FILE__,__line__=__LINE__) && 0 ? NULL : new
