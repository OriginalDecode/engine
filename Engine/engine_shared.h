#pragma once
#include <standard_datatype.hpp>
#include <CommonLib/HashString.h>

#include "EngineDefines.h"

#ifdef _PROFILE
#include <easy/profiler.h>
#include <easy/reader.h>
#endif

#include <Windows.h>

#define BITFLAG(x) (1 << x)

typedef s32 Entity;

#if (UINTPTR_MAX == 0xffffffffffffffff)
typedef u64 uptr;
#else
typedef u32 uptr;
#endif

enum class eShaderType : s32
{
	VERTEX,
	PIXEL,
	GEOMETRY,
	HULL,
	DOMAINS,
	COMPUTE,
	_COUNT
};

struct Ticket_Mutex
{
	u64 volatile ticket = 0;
	u64 volatile serving = 0;
};

inline u64 AtmoicAddU64(u64 volatile *value, u64 toAdd)
{
	u64 result = _InterlockedExchange64((__int64 volatile *)value, (*value + toAdd));
	return result;
};

inline void BeginTicketMutex(Ticket_Mutex* mutex)
{
	u64 ticket = AtmoicAddU64(&mutex->ticket, 1); //AtomicAdd?
	while (ticket != mutex->serving)
	{
	
	}
};

inline void EndTicketMutex(Ticket_Mutex* mutex)
{
	mutex->serving++;
};