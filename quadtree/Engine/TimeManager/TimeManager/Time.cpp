#include "Time.h"

namespace CommonUtilities
{
		Time::Time()
		{
			myDeltaSeconds = 0;
		}


		Time::~Time()
		{
		}

		void Time::Update(LARGE_INTEGER& aLastEnd, LARGE_INTEGER& anEnd, LARGE_INTEGER& aClockFreq)
		{
			myDeltaSeconds = (anEnd.QuadPart - aLastEnd.QuadPart) / static_cast<float>(aClockFreq.QuadPart);
		}

		float Time::GetHours()
		{
			return static_cast<float>((myDeltaSeconds / 60.f) / 60.f);
		}

		float Time::GetMinutes()
		{
			return static_cast<float>(myDeltaSeconds / 60.f);
		}

		float Time::GetSeconds()
		{
			return myDeltaSeconds;
		}

		float Time::GetMilliseconds()
		{
			return static_cast<float>(myDeltaSeconds / 1000.f);
		}

		float Time::GetMicroseconds()
		{
			return static_cast<float>(myDeltaSeconds / 1000000.f);
		}

		float Time::GetFPS()
		{
			return 1 / myDeltaSeconds;
		}

		void Time::SetTime(float someTime)
		{
			myDeltaSeconds = someTime;
		}
}