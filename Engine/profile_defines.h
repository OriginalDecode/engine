#pragma once
#include "easy/profiler_colors.h"
#ifdef _PROFILE
#include "easy/profiler.h"
#include "easy/reader.h"

#define PROFILE_FUNCTION(color) EASY_FUNCTION(color)
#define PROFILE_BLOCK(name, ...) EASY_BLOCK(name, __VA_ARGS__)
#define PROFILE_BLOCK_END EASY_END_BLOCK

#undef EASY_FUNCTION
#undef EASY_BLOCK
#undef EASY_END_BLOCK

#else
#define PROFILE_FUNCTION(color) 
#define PROFILE_BLOCK(name, ...) 
#define PROFILE_BLOCK_END

#undef EASY_FUNCTION
#undef EASY_BLOCK
#undef EASY_END_BLOCK
#endif