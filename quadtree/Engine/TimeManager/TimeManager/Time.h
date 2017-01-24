#pragma once
#include <Windows.h>
#include <iostream>
namespace CommonUtilities
{
	class Time
	{
	public:
		Time();
		~Time();

		void Update(LARGE_INTEGER& aLastEnd, LARGE_INTEGER& anEnd, LARGE_INTEGER& aClockFreq);

		float GetHours();
		float GetMinutes();
		float GetSeconds();
		float GetMilliseconds();
		float GetMicroseconds();
		float GetFPS();

		void SetTime(float someTime);

	private:

		float myDeltaSeconds;

	};
}