#pragma once
#include <Windows.h>

#define T_FLOAT

#ifdef T_FLOAT
typedef float time_type;
#endif
#ifndef T_FLOAT
typedef double time_type;
#endif




namespace CommonUtilities
{
	class Time
	{
	public:
		Time();
		~Time();

		void Update(LARGE_INTEGER& aLastEnd, LARGE_INTEGER& anEnd, LARGE_INTEGER& aClockFreq);

		time_type GetHours();
		time_type GetMinutes();
		time_type GetSeconds();
		time_type GetMilliseconds();
		time_type GetMicroseconds();
		time_type GetFPS();

		void SetTime(time_type someTime);

	private:

		time_type myDeltaSeconds;

	};
}