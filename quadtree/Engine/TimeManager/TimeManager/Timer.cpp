#include "Timer.h"
namespace CommonUtilities
{
	Timer::Timer()
	{
		myCurrentActive = false;
		QueryPerformanceFrequency(&myClockFreq);
	}

	Timer::~Timer()
	{
	}

	void Timer::Update()
	{
		if (myCurrentActive == true)
		{
			QueryPerformanceCounter(&myEnd);
			myTime.Update(myLastEnd, myEnd, myClockFreq);
			myTotalTime.Update(myStart, myEnd, myClockFreq);
			myLastEnd = myEnd;
		}
		else
		{
			QueryPerformanceCounter(&myEnd);
			myTime.Update(myEnd, myEnd, myClockFreq);
			myLastEnd = myEnd;
		}
	}

	void Timer::Start()
	{
		myCurrentActive = true;
		QueryPerformanceCounter(&myStart);
		myLastEnd = myStart;
		myEnd = myStart;

	}

	void Timer::Stop()
	{
		myCurrentActive = false;
	}

	void Timer::Pause()
	{
		myCurrentActive = false;
	}

	void Timer::Resume()
	{
		myLastEnd = myEnd;
		QueryPerformanceCounter(&myEnd);
		myStart.QuadPart += (myEnd.QuadPart - myLastEnd.QuadPart);
		myCurrentActive = true;
	}

	Time Timer::GetFrameTime()
	{
		return myTime;
	}

	Time Timer::GetTotalTime()
	{
		return myTotalTime;
	}

	void Timer::ClearTime()
	{
		myTotalTime.SetTime(0);
	}
}