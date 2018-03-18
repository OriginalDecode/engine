
// #include <iostream>
// #include <new>

// #include <map>


// struct memory_data
// {
// 	memory_data(void* p, const char* f, size_t l)
// 		: pointer(p)
// 		, file(f)
// 		, line(l)
// 	{
// 	}
// 	void* pointer;
// 	const char* file;
// 	size_t line;
// };

// static std::map<void*, memory_data> g_Memory;

// static int allocations = 0;
// void record_alloc(void* p, const char* file, size_t line)
// {
// 	char temp[100];

// //	g_Memory.insert(std::make_pair(p, memory_data(p, file, line)));


// 	 sprintf_s(temp, "\nAllocation\n%x\n%s\n%ld\n", p, file, line);
// 	// OutputDebugString(temp);
// }

// void unrecord_alloc(void* p)
// {
// 	//auto& it = g_Memory.find(p);
// 	//g_Memory.erase(it);

// 	 char temp[100];
// 	 sprintf_s(temp , "\nDealloc\n%x\n", p);
// 	// OutputDebugString(temp);
// }


// char* __file__ = "unknown";
// size_t __line__ = 0;

// void* operator new(size_t n)
// {
// 	allocations++;
// 	void * p = malloc(n);
// 	record_alloc(p, __file__, __line__);
// 	__file__ = "unknown";
// 	__line__ = 0;

// 	return p;
// }

// void* operator new[](size_t n)
// {
// 	allocations++;
// 	void * p = malloc(n);
// 	record_alloc(p, __file__, __line__);
// 	__file__ = "unknown";
// 	__line__ = 0;

// 	return p;
// }

// void operator delete(void* p)
// {
// 	allocations--;
// 	unrecord_alloc(p);
// 	free(p);
// }

// void operator delete[](void* p)
// {
// 	allocations--;
// 	unrecord_alloc(p);
// 	free(p);
// }

// #define new (__file__=__FILE__,__line__=__LINE__) && 0 ? NULL : new
