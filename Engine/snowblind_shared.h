#pragma once
#include <Windows.h>
#include <standard_datatype.hpp>

struct Ticket_Mutex
{
	u64 volatile ticket = 0;
	u64 volatile serving = 0;
};

inline u64 AtmoicAddU64(u64 volatile *value, u64 toAdd)
{
	u64 result = _InterlockedExchange64((__int64 volatile *)value, toAdd);
	return result;
};



inline void BeginTicketMutex(Ticket_Mutex* mutex)
{
	u64 ticket = AtmoicAddU64(&mutex->ticket, 1); //AtomicAdd?
	while (ticket != mutex->serving);
}

inline void EndTicketMutex(Ticket_Mutex* mutex)
{
	AtmoicAddU64(&mutex->serving, 1); //AtomicAdd?
}

