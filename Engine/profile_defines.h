#pragma once

#ifndef BUILD_WITH_EASY_PROFILER
#define BUILD_WITH_EASY_PROFILER
#endif

#include "../easy/profiler_colors.h"
#ifdef BUILD_WITH_EASY_PROFILER
#ifdef _PROFILE
#include "../easy/profiler.h"
#include "../easy/reader.h"

#define PROFILE_FUNCTION(color) EASY_FUNCTION(color)
#define PROFILE_BLOCK(name, ...) EASY_BLOCK(name, __VA_ARGS__)
#define PROFILE_BLOCK_END EASY_END_BLOCK
#define PROFILE_THREAD_SCOPE(name) EASY_THREAD_SCOPE(x);

#else
#define PROFILE_FUNCTION(color) 
#define PROFILE_BLOCK(name, ...) 
#define PROFILE_BLOCK_END
#define PROFILE_THREAD_SCOPE(name)

#undef EASY_FUNCTION
#undef EASY_BLOCK
#undef EASY_END_BLOCK
#undef EASY_THREAD_SCOPE
#endif
#endif