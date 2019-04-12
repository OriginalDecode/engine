#include "Timer.h"
#include <cassert>
#include <ctime>
namespace Base
{
	Timer::Timer()
		: m_IsActive(false)
		, m_IsPaused(true)
	{
	}


	void Timer::Update()
	{
		
		m_Prev = m_Current;
		if (time(&m_Current) == -1)
			assert(!"Failed to get time!");

		m_Time = (double)m_Prev - (double)m_Current;


		//LARGE_INTEGER current = ((m_IsActive == false) && m_IsPaused) ? m_Prev : m_Current;

		//m_TotalTime.Update(m_Start, current, m_Frequency);
		//m_CurrentTime.Update(m_Prev, current, m_Frequency);
		//m_Prev = current;
	}

	void Timer::Start()
	{
	

		if (time(&m_Start) == -1)
			assert(!"Failed to get time!");
		m_Current = m_Start;
		m_Prev = m_Start;

		/*m_IsActive = true;

		QueryPerformanceCounter(&m_Start);
		myLastEnd = myStart;
		myEnd = myStart;
*/
	}

	void Timer::Stop()
	{
		//myCurrentActive = false;
	}

	void Timer::Pause()
	{
		//myCurrentActive = false;
	}

	void Timer::Resume()
	{
		/*m_Prev = m_Current;
		QueryPerformanceCounter(&myEnd);
		myStart.QuadPart += (myEnd.QuadPart - myLastEnd.QuadPart);
		myCurrentActive = true;*/
	}

	//const Time& Timer::GetTime() const
	//{
	//	return m_CurrentTime;
	//}

	//const Time& Timer::GetTotalTime() const
	//{
	//	return m_TotalTime;
	//}

	void Timer::ClearTime()
	{
	}

	float Timer::GetTime()
	{
		return m_Time;
	}

}