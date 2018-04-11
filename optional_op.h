//#ifndef MEM_TRACE
//#define MEM_TRACE
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <memory>
//#include <new>
//#include <map>
//#include "../DL_Debug/StackWalker/StackWalker.h"
//
//struct memory_data
//{
//	memory_data() = default;
//	memory_data(void* p, const char* f, size_t l, size_t s)
//		: pointer(p)
//		, file(f)
//		, line(l)
//		, size(s)
//	{
//	}
//	void* pointer;
//	const char* file;
//	size_t line;
//	size_t size;
//	std::string callstack[4];
//	//CallstackEntryList callstack;
//};
//
//
//template<typename T>
//struct track_alloc : std::allocator<T>
//{
//	typedef typename std::allocator<T>::pointer pointer;
//	typedef typename std::allocator<T>::size_type size_type;
//
//	template<typename U>
//	struct rebind
//	{
//		typedef track_alloc<U> other;
//	};
//
//	track_alloc()
//	{
//	}
//
//	template<typename U>
//	track_alloc(track_alloc<U> const& u) : std::allocator<T>(u)
//	{
//	}
//
//	pointer allocate(size_type size, std::allocator<void>::const_pointer = 0)
//	{
//		void* p = malloc(size * sizeof(T));
//		if (p == 0)
//			throw std::bad_alloc();
//
//		return static_cast<pointer>(p);
//	}
//
//	void deallocate(pointer p, size_type)
//	{
//		free(p);
//	}
//};
//
//
//typedef std::map< void*, memory_data, std::less<void*>, track_alloc<std::pair<void* const, size_t> > > track_type;
//
//
//struct track_printer
//{
//	track_type* track;
//	track_printer(track_type* track)
//		:track(track)
//	{
//	}
//
//	~track_printer()
//	{
//		track_type::const_iterator it = track->begin();
//		std::ofstream leak_track("leaks.txt", std::ios_base::trunc);
//		while (it != track->end())
//		{
//			std::string file(it->second.file);
//
//
//			if (!file.empty())
//			{
//				size_t pos = file.rfind('/');
//				file = file.substr(pos + 1);
//			}
//			leak_track << "\nLeak at " << it->first << " size: " << it->second.size << " file: " << file << " line: " << it->second.line << "\n" << "callstack : \n";
//
//			std::string entry = it->second.callstack[0];
//			leak_track << entry << "\n";
//
//			entry = it->second.callstack[1];
//			leak_track << entry << "\n";
//
//			entry = it->second.callstack[2];
//			leak_track << entry << "\n";
//
//			entry = it->second.callstack[3];
//			leak_track << entry << "\n";
//
//
//			++it;
//		}
//		leak_track.close();
//	}
//};
//
//track_type * get_map()
//{
//	// don't use normal new to avoid infinite recursion.
//	static track_type* track = new (malloc(sizeof *track)) track_type;
//	static track_printer printer(track);
//	return track;
//};
//
//char* __file__ = "unknown";
//size_t __line__ = 0;
//
//void* operator new(size_t n)
//{
//	void * p = malloc(n);
//
//	//this shit calls new
//	BaseStackWalker walker;
//	walker.ShowCallstack(4);
//
//	memory_data data;
//	data.file = __file__;
//	data.line = __line__;
//	data.pointer = p;
//	data.size = n;
//	for (int i = 0; i < 4; i++)
//	{
//		data.callstack[i] = walker.GetEntry(i);
//	}
//
//
//	(*get_map())[p] = memory_data(p, __file__, __line__, n);
//	__file__ = "unknown";
//	__line__ = 0;
//
//
//
//	return p;
//}
//
//void* operator new[](size_t n)
//{
//	void * p = malloc(n);
//	(*get_map())[p] = memory_data(p, __file__, __line__, n);
//	__file__ = "unknown";
//	__line__ = 0;
//
//	return p;
//}
//
//void operator delete(void* p)
//{
//	(*get_map()).erase(p);
//	free(p);
//}
//
//void operator delete[](void* p)
//{
//	(*get_map()).erase(p);
//	free(p);
//}
//
//#define new (__file__=__FILE__,__line__=__LINE__) && 0 ? NULL : new
//
//#endif