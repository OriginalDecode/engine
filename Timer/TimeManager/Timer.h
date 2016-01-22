#pragma once
#include <Windows.h>
#include <iostream>
#include "Time.h"
namespace CommonUtilities
{
	class Timer
	{
	public:
		Timer();
		~Timer();


		void Update();

		void Start();
		void Stop();
		void Pause();
		void Resume();

		void ClearTime();

		Time GetFrameTime();
		Time GetTotalTime();


	private:

		Time myTime;
		Time myTotalTime;

		LARGE_INTEGER myClockFreq;
		LARGE_INTEGER myStart;

		LARGE_INTEGER myEnd;
		LARGE_INTEGER myLastEnd;

		bool myCurrentActive;
	};
}
